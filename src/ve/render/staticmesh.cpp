#include "staticmesh.h"

StaticMesh::StaticMesh() {}

StaticMesh::StaticMesh(const VertexArray& va, const IndexBuffer& ib, Ref<Material> material) {
    m_subMeshes.push_back(MeshData(va, ib, material));
}

StaticMesh::StaticMesh(const std::vector<MeshData>& subMeshes)
    : m_subMeshes(subMeshes) {}

StaticMesh::StaticMesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, Ref<Material> material) {
    m_subMeshes.push_back(MeshData(
        VertexArray(vertices),
        IndexBuffer(indices),
        material
    ));
}