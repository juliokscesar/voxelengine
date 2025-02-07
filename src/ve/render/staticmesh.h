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

    inline const std::vector<MeshData>& subMeshes() const { return m_subMeshes; }

private:
    std::vector<MeshData> m_subMeshes;
};

namespace PrimitiveMesh {
inline StaticMesh cube() {
    static bool isFirstCall = true;
    static StaticMesh cubeMesh;
    if (!isFirstCall)
        return cubeMesh;

    std::vector<Vertex> vertices = {
        // Front face
        {{-0.5f, -0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
        {{0.5f, -0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},
        {{0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
        {{-0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
        // Back face
        {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f}},
        {{0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f}},
        {{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f}},
        {{-0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f}}
    };

    std::vector<uint32_t> indices = {
        0, 1, 2, 2, 3, 0, // Front
        4, 5, 6, 6, 7, 4, // Back
        0, 4, 7, 7, 3, 0, // Left
        1, 5, 6, 6, 2, 1, // Right
        3, 2, 6, 6, 7, 3, // Top
        0, 1, 5, 5, 4, 0  // Bottom
    };

    cubeMesh = StaticMesh(vertices, indices, Material::blank());
    isFirstCall = false;
    return cubeMesh;
}
} // namespace PrimitiveMesh
