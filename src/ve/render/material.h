#pragma once

#include "core/base.h"
#include "texture.h"
#include "shader.h"

#include <glm/vec3.hpp>

struct Material {
    std::vector<Ref<Texture2D>> diffuse;
    std::vector<Ref<Texture2D>> specular;
    glm::vec3 ambient;
    float shininess;
    float tilingFactor;

    // If this is null, the global default shader is used
    Ref<Shader> shader = nullptr;

    bool useLighting;

    static Ref<Material> fromColor(const glm::u8vec4& rgba) {
        Ref<Material> material = createRef<Material>();
        material->diffuse.push_back(createRef<Texture2D>(Texture2D::fromColor(rgba, 640, 640, 0)));
        material->specular.push_back(createRef<Texture2D>(Texture2D::fromColor(rgba, 640, 640, 1)));
        material->ambient = glm::vec3(0.4f);
        material->shininess = 0.3f;
        material->tilingFactor = 1.0f;
        return material;
    }
};
