#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in uint aMatID;

//out vec3 fragPos;
//out vec3 fragNormal;
out vec2 texCoords;
out uint matID;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    // TODO: figure out how to do all of this in CPU. becasue this is done for every fragment
    //fragNormal = mat3(transpose(inverse(model))) * aNormal;
    vec3 fragPos = vec3(model * vec4(aPos, 1.0));
    texCoords = aTexCoords;
    matID = aMatID;

    gl_Position = projection * view * vec4(fragPos, 1.0);
}
