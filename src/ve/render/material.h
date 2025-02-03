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

    Ref<Shader> shader;

    void enableMaterialShader();
    bool useLighting;

    static Ref<Material> blank() {
        static bool isFirstCall = true;
        static Ref<Material> blankMat;
        if (!isFirstCall)
            return blankMat;

        // create white texture
        Texture2DProperties texProps(640,640,GL_RGB,GL_RGBA);
        std::vector<uint8_t> whiteData(texProps.width*texProps.height*4,255);
        blankMat = createRef<Material>();
        blankMat->diffuse.push_back(createRef<Texture2D>(whiteData.data(), texProps));
        blankMat->specular.push_back(createRef<Texture2D>(whiteData.data(), texProps, 1));
        blankMat->ambient = glm::vec3(0.4f);
        blankMat->shininess = 0.3f;
        blankMat->tilingFactor = 1.0f;
        isFirstCall = false;
        return blankMat;
    }
};
