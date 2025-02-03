#include "resource_manager.h"

#include "utils/file_handler.h"
#include "logging.h"

ResourceManager::ResourceManager() {
}

ResourceManager::~ResourceManager() {
    for (auto& [path,tex] : m_tex2DHistory) {
        tex->clear();
    }
    for (auto& [path, sh] : m_shaderHistory) {
        sh->clear();
    }
}

Ref<Texture2D> ResourceManager::loadTexture(const stdfs::path &path, uint32_t unit) {
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
            grflog::fatal("Unable to translate {} number of channels into a proper GL pixel format (path: {})", texData.nChannels, path.string());
            return nullptr;
    }

    tex = createRef<Texture2D>(texData.data, texProps, unit);
    m_tex2DHistory[path] = tex;
    return tex;
}

bool ResourceManager::findTexture(const stdfs::path& path, Ref<Texture2D>* dest) {
    if (auto t = m_tex2DHistory.find(path); t != m_tex2DHistory.end()) {
        if (dest)
            *dest = t->second;
        return true;
    }
    return false;
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