#pragma once

#include "base.h"

#include "render/texture.h"
#include "render/material.h"
#include "render/shader.h"

class ResourceManager {
public:
    ResourceManager(const Texture2DProperties& defaultTexProps);
    ~ResourceManager();

    Ref<Texture2D> createTexture(const uint8_t* data, const Texture2DProperties& props);
    Ref<Texture2D> createTexture(const glm::u8vec4& rgba, uint32_t width, uint32_t height);
    Ref<Texture2D> loadTexture(const stdfs::path& path);
    bool findTexture(const stdfs::path& path, Ref<Texture2D>* dest);
    bool isTextureValid(Ref<Texture2D> tex);

    Ref<Material> createMaterial(Ref<Texture2D> diffuse, Ref<Texture2D> specular, const glm::vec3& ambient, float tilingFactor, bool isLit);
    Ref<Material> createMaterial(const glm::u8vec4& rgba, const glm::vec3& ambient, bool isLit);

    Ref<Shader> loadShader(const stdfs::path& vertPath, const stdfs::path& fragPath);
    bool findShader(const stdfs::path& vertPath, Ref<Shader>* dest);

    void loadGPUData();

private:
    uint32_t m_texArray;
    uint32_t m_mipMapLevels = 1;
    std::vector<Ref<Texture2D>> m_textures;
    Texture2DProperties m_defaultTexProps;
    std::unordered_map<stdfs::path, Ref<Texture2D>> m_localTexHistory;
    std::vector<uint8_t> m_texDataBuffer;

    std::vector<Ref<Material>> m_materials;
    uint32_t m_matSSBO;

    std::unordered_map<stdfs::path, Ref<Shader>> m_shaderHistory;
};
