#pragma once

#include "core/base.h"

#include "material.h"
#include "vertex.h"

struct MeshData {
    VertexArray va;
    IndexBuffer ib;
    Ref<Material> material;

    MeshData();
    MeshData(const VertexArray& va, const IndexBuffer& ib, Ref<Material> material);
};

class StaticMesh {
public:
    StaticMesh();
    StaticMesh(const VertexArray& va, const IndexBuffer& ib, Ref<Material> material);
    StaticMesh(const std::vector<MeshData>& subMeshes);
    StaticMesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, Ref<Material> material);

    std::vector<MeshData> subMeshes;
};

namespace PrimitiveMesh {
inline StaticMesh cube(const glm::u8vec4& matColor = glm::u8vec4(255)) {
    static bool isFirstCall = true;
    static StaticMesh cubeMesh;
    if (!isFirstCall) {
        cubeMesh.subMeshes[0].material = Material::fromColor(matColor);
        return cubeMesh;
    }

    std::vector<Vertex> vertices = {
        // Front face
        {{-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f,  1.0f}, {0.0f, 0.0f}},  
        {{ 0.5f, -0.5f,  0.5f}, {0.0f, 0.0f,  1.0f}, {1.0f, 0.0f}},  
        {{ 0.5f,  0.5f,  0.5f}, {0.0f, 0.0f,  1.0f}, {1.0f, 1.0f}},  
        {{-0.5f,  0.5f,  0.5f}, {0.0f, 0.0f,  1.0f}, {0.0f, 1.0f}},  

        // Right face
        {{ 0.5f, -0.5f,  0.5f}, {1.0f, 0.0f,  0.0f}, {0.0f, 0.0f}},  
        {{ 0.5f, -0.5f, -0.5f}, {1.0f, 0.0f,  0.0f}, {1.0f, 0.0f}},  
        {{ 0.5f,  0.5f, -0.5f}, {1.0f, 0.0f,  0.0f}, {1.0f, 1.0f}},  
        {{ 0.5f,  0.5f,  0.5f}, {1.0f, 0.0f,  0.0f}, {0.0f, 1.0f}},  

        // Back face
        {{ 0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f}},  
        {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f}},  
        {{-0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f}},  
        {{ 0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f}},  

        // Left face
        {{-0.5f, -0.5f, -0.5f}, {-1.0f, 0.0f,  0.0f}, {0.0f, 0.0f}},  
        {{-0.5f, -0.5f,  0.5f}, {-1.0f, 0.0f,  0.0f}, {1.0f, 0.0f}},  
        {{-0.5f,  0.5f,  0.5f}, {-1.0f, 0.0f,  0.0f}, {1.0f, 1.0f}},  
        {{-0.5f,  0.5f, -0.5f}, {-1.0f, 0.0f,  0.0f}, {0.0f, 1.0f}},  

        // Top face
        {{-0.5f,  0.5f,  0.5f}, {0.0f, 1.0f,  0.0f}, {0.0f, 0.0f}},  
        {{ 0.5f,  0.5f,  0.5f}, {0.0f, 1.0f,  0.0f}, {1.0f, 0.0f}},  
        {{ 0.5f,  0.5f, -0.5f}, {0.0f, 1.0f,  0.0f}, {1.0f, 1.0f}},  
        {{-0.5f,  0.5f, -0.5f}, {0.0f, 1.0f,  0.0f}, {0.0f, 1.0f}},  

        // Bottom face
        {{-0.5f, -0.5f, -0.5f}, {0.0f, -1.0f,  0.0f}, {0.0f, 0.0f}},  
        {{ 0.5f, -0.5f, -0.5f}, {0.0f, -1.0f,  0.0f}, {1.0f, 0.0f}},  
        {{ 0.5f, -0.5f,  0.5f}, {0.0f, -1.0f,  0.0f}, {1.0f, 1.0f}},  
        {{-0.5f, -0.5f,  0.5f}, {0.0f, -1.0f,  0.0f}, {0.0f, 1.0f}}   
    };

    std::vector<uint32_t> indices = {
        0, 1, 3, 3, 1, 2,  // Front
        4, 5, 7, 7, 5, 6,  // Right
        8, 9, 11, 11, 9, 10, // Back
        12, 13, 15, 15, 13, 14, // Left
        16, 17, 19, 19, 17, 18, // Top
        20, 21, 23, 23, 21, 22  // Bottom
    };

    cubeMesh = StaticMesh(vertices, indices, Material::fromColor(matColor));
    isFirstCall = false;
    return cubeMesh;
}
} // namespace PrimitiveMesh
