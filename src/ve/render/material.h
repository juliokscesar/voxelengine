#pragma once

#include "core/base.h"
#include "texture.h"
#include "shader.h"

#include <glm/vec3.hpp>

struct GPUMaterial {
    uint32_t diffuseTexID;
    uint32_t specularTexID;
    glm::vec3 ambient;
    float shininess;
    float tilingFactor;
    bool isLit;

    GPUMaterial(uint32_t diffuseTexID, uint32_t specularTexID, const glm::vec3& ambient, float shininess, float tilingFactor, bool isLit);
};

struct Material {
    GPUMaterial gpuMat;
    uint32_t ID;

    // If this is null, the global default shader is used
    Ref<Shader> shader = nullptr;

    Material(uint32_t id, Ref<Texture2D> diffuse, Ref<Texture2D> specular, const glm::vec3& ambient, float shininess, float tilingFactor, bool isLit);

    // TODO: take this out of here
    // static Ref<Material> fromColor(const glm::u8vec4& rgba) {
    //     Ref<Material> material = createRef<Material>();
    //     material->diffuse = createRef<Texture2D>(Texture2D::fromColor(rgba, 640, 640, 0));
    //     material->specular = createRef<Texture2D>(Texture2D::fromColor(rgba, 640, 640, 1));
    //     material->ambient = glm::vec3(0.4f);
    //     material->shininess = 0.3f;
    //     material->tilingFactor = 1.0f;
    //     return material;
    // }
};
