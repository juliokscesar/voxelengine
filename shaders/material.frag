#version 460 core

struct Material {
    uint diffuse;
    uint specularMap;
    vec3 ambient;
    float shininess;
    float tilingFactor;
    bool isLit;
};
layout (std430, binding = 0) buffer MaterialBlock {
    Material materials[];
};

uniform sampler2DArray textureArray;

//uniform bool useMaterial;

//uniform vec3 viewPos;

// from material.vert
//in vec3 fragPos;
//in vec3 fragNormal;
flat in uint matID;
in vec2 texCoords;

out vec4 FragColor;
void main() {
    Material mat = materials[matID];
    
    vec4 texColor = texture(textureArray, vec3(texCoords, mat.diffuse));
    if (texColor.a < 0.5)
        discard;

    FragColor = texColor;
}
