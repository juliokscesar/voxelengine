#include "material.h"

GPUMaterial::GPUMaterial(uint32_t diffuseTexID, uint32_t specularTexID, const glm::vec3& ambient, float shininess, float tilingFactor, bool isLit)
    : diffuseTexID(diffuseTexID)
    , specularTexID(specularTexID)
    , ambient(ambient)
    , shininess(shininess)
    , tilingFactor(tilingFactor)
    , isLit(isLit) {}

Material::Material(uint32_t id, Ref<Texture2D> diffuse, Ref<Texture2D> specular,
                   const glm::vec3& ambient, float shininess, float tilingFactor, bool isLit)
    : ID(id) 
    , gpuMat(diffuse->getID(), specular->getID(), ambient, shininess, tilingFactor, isLit) {}
