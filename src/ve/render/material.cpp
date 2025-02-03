#include "material.h"

#include "core/logging.h"

void Material::enableMaterialShader() {
    shader->use();
    const std::string uMatFmt = "material.";
    shader->setUniformVec3(uMatFmt+"ambient", ambient);
    shader->setUniformFloat(uMatFmt+"tilingFactor", tilingFactor);
    // set diffuse maps in even numbered units
    size_t size = static_cast<uint32_t>(diffuse.size());
    std::string nameFmt = uMatFmt+".diffuseMaps[";
    for (uint32_t i = 0, u = 0; i < size; i++) {
        Ref<Texture2D> diffMap = diffuse[i];
        diffMap->setUnit(u);
        diffMap->bind();

        shader->setUniformUInt(nameFmt + std::to_string(i) + "]", u);

        u += 2;
    }

    // only set specular and shininess if using light
    if (useLighting) {
        shader->setUniformFloat(uMatFmt+"shininess", shininess);

        size = static_cast<uint32_t>(specular.size());
        nameFmt = uMatFmt+".specularMaps[";
        // specular goes in odd numbered units
        for (uint32_t i = 0, u = 1; i < size; i++) {
            Ref<Texture2D> specMap = specular[i];
            specMap->setUnit(u);
            specMap->bind();

            shader->setUniformUInt(nameFmt + std::to_string(i) + "]", u);

            u += 2;
        }
    }
}
