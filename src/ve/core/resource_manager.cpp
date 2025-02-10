#include "resource_manager.h"

#include "utils/image.h"
#include "utils/file_handler.h"
#include "logging.h"

ResourceManager::ResourceManager(const Texture2DProperties& defaultTexProps)
    : m_defaultTexProps(defaultTexProps) {
    // first initialize texture array in the gpu
    glGenTextures(1, &m_texArray);
    glBindTexture(GL_TEXTURE_2D_ARRAY, m_texArray);

    // initialize material ssbo
    glGenBuffers(1, &m_matSSBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_matSSBO);
}

ResourceManager::~ResourceManager() {
    if (m_texArray) {
        glDeleteTextures(1, &m_texArray);
        m_texArray = 0;
    }
    for (auto& [path, sh] : m_shaderHistory) {
        sh->clear();
    }
}

Ref<Texture2D> ResourceManager::createTexture(const uint8_t* data, const Texture2DProperties& props) {
    // store data in memory, load to the GPU all at once when starting
    m_texDataBuffer.reserve(m_texDataBuffer.size() + props.width * props.height);
    std::copy(data, data + props.width * props.height, m_texDataBuffer.end());

    Texture2DProperties texProps(props);
    texProps.storeFmt = m_defaultTexProps.storeFmt;
    Ref<Texture2D> tex = createRef<Texture2D>(m_textures.size(), m_texArray, texProps);
    m_textures.push_back(tex);
    return tex;
}

Ref<Texture2D> ResourceManager::createTexture(const glm::u8vec4& rgba, uint32_t width, uint32_t height) {
    utils::ImageData img(width, height, 4);
    utils::fillSingleColor(img, rgba);

    Texture2DProperties props(width, height, m_defaultTexProps.storeFmt, GL_RGBA);
    Ref<Texture2D> tex = createTexture(img.data, props);
    return tex;
}

Ref<Texture2D> ResourceManager::loadTexture(const stdfs::path &path) {
    if (!utils::existPaths(path)) {
        grflog::critical("Path for texture {} doesn't exist", path.string());
        return nullptr;
    }

    Ref<Texture2D> tex;
    if (findTexture(path, &tex))
        return tex;
    
    utils::ImageData texData(path);
    if (!texData.data) {
        grflog::critical("Unable to load image {}", path.string());
        return nullptr;
    }

    Texture2DProperties texProps;
    texProps.width = texData.width;
    texProps.height = texData.height;
    texProps.storeFmt = GL_RGB;
    switch (texData.nChannels) {
        case 1:     texProps.pixelFmt = GL_RED;     break;
        case 2:     texProps.pixelFmt = GL_RG;      break;
        case 3:     texProps.pixelFmt = GL_RGB;     break;
        case 4:     texProps.pixelFmt = GL_RGBA;    break;
        
        default:
            grflog::fatal("Unable to translate {} channels into a proper GL pixel format (path: {})", texData.nChannels, path.string());
            return nullptr;
    }

    tex = createTexture(texData.data, texProps);
    m_localTexHistory[path] = tex;
    return tex;
}

bool ResourceManager::findTexture(const stdfs::path& path, Ref<Texture2D>* dest) {
    if (auto t = m_localTexHistory.find(path); t != m_localTexHistory.end()) {
        if (dest)
            *dest = t->second;
        return true;
    }
    return false;
}

bool ResourceManager::isTextureValid(Ref<Texture2D> tex) { 
    return (std::find(m_textures.begin(), m_textures.end(), tex) != m_textures.end());
}

Ref<Material> ResourceManager::createMaterial(Ref<Texture2D> diffuse,
                                              Ref<Texture2D> specular,
                                              const glm::vec3& ambient,
                                              float tilingFactor, bool isLit) {
    if (!isTextureValid(diffuse)) {
        grflog::critical("Trying to create material with invalid/unloaded diffuse texture");
        return nullptr;
    }
    if (!isTextureValid(specular)) {
        grflog::critical("Trying to create material with invalid/unloaded specular texture. Using diffuse in place");
        specular = diffuse;
    }

    const uint32_t id = static_cast<uint32_t>(m_materials.size());
    Ref<Material> material = createRef<Material>(id, diffuse, specular, ambient, tilingFactor, isLit);
    m_materials.push_back(material);
    
    return material;
}

Ref<Material> ResourceManager::createMaterial(const glm::u8vec4& rgba,
                                              const glm::vec3& ambient,
                                              bool isLit) {
    return Ref<Material>();
}

Ref<Shader> ResourceManager::loadShader(const stdfs::path &vertPath, const stdfs::path &fragPath) {
    Ref<Shader> shader;
    if (findShader(vertPath, &shader))
        return shader;
    
    shader = createRef<Shader>(vertPath, fragPath, true /* compileOnLoad */);
    m_shaderHistory[vertPath] = shader;
    return shader;
}

bool ResourceManager::findShader(const stdfs::path& vertPath, Ref<Shader>* dest) {
    if (auto s = m_shaderHistory.find(vertPath); s != m_shaderHistory.end()) {
        if (dest)
            *dest = s->second;
        return true;
    }
    return false;
}

void ResourceManager::loadGPUData() {
    // first load Texture Array
    glBindTexture(GL_TEXTURE_2D_ARRAY, m_texArray);
    // TODO: more than 1 mipmap

    const uint32_t width = m_defaultTexProps.width;
    const uint32_t height = m_defaultTexProps.height;
    const size_t layerCount = m_textures.size();
    const GLenum fmt = m_defaultTexProps.storeFmt;

    glTexStorage3D(GL_TEXTURE_2D_ARRAY, m_mipMapLevels, fmt, width, height, layerCount);
    glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, 0, m_defaultTexProps.width, m_defaultTexProps.height, layerCount, fmt, GL_UNSIGNED_BYTE, m_texDataBuffer.data());
    
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // clear buffer data after sent to the gpu
    m_texDataBuffer.clear();

    // load materials
    std::vector<GPUMaterial> gpuData;
    gpuData.reserve(m_materials.size());
    for (const auto& mat : m_materials) {
        if (mat)
            gpuData.push_back(mat->gpuMat);
    }
    const size_t matSize = gpuData.size();

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_matSSBO);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(GPUMaterial) * matSize, gpuData.data(), GL_STATIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_matSSBO);
}
