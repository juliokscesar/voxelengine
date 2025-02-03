#pragma once

#include "base.h"

#include "render/texture.h"
#include "render/shader.h"

class ResourceManager {
public:
    ResourceManager();
    ~ResourceManager();

    Ref<Texture2D> loadTexture(const stdfs::path& path, uint32_t unit = 0);
    bool findTexture(const stdfs::path& path, Ref<Texture2D>* dest);

    Ref<Shader> loadShader(const stdfs::path& vertPath, const stdfs::path& fragPath);
    bool findShader(const stdfs::path& vertPath, Ref<Shader>* dest);

private:
    std::unordered_map<stdfs::path, Ref<Texture2D>> m_tex2DHistory;
    std::unordered_map<stdfs::path, Ref<Shader>> m_shaderHistory;
};
