#pragma once

#include "core/base.h"
#include "texture.h"
#include "shader.h"

#include <glm/vec3.hpp>

struct Material {
    std::vector<Texture2D> diffuse;
    std::vector<Texture2D> specular;
    glm::vec3 ambient;
    float shininess;
    float tilingFactor;

    Ref<Shader> shader;
};
