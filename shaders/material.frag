#version 460 core

#define MAX_NUMBER_SAMPLER2D 15

// from material.vert
//in vec3 fragPos;
//in vec3 fragNormal;
in vec2 texCoords;

struct Material {
    sampler2D diffuseMaps[MAX_NUMBER_SAMPLER2D];
    //sampler2D specularMaps[MAX_NUMBER_SAMPLER2D];
    vec3 ambient;
    //float shininess;
    float tilingFactor;
};
uniform Material material;
//uniform bool useMaterial;

//uniform vec3 viewPos;

out vec4 fragColor;
void main() {
    vec2 fragTexCoords  = texCoords * material.tilingFactor;

    // TODO: lighting

    // TODO: implement for more than 1 diffuse and specular texture?
    vec4 fragTex = texture(material.diffuseMaps[0], fragTexCoords);
    if (fragTex.a < 0.5)
        discard;

    fragColor = fragTex * vec4(material.ambient, 1.0);
}
