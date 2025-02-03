#include "staticmesh.h"

StaticMesh::StaticMesh(const VertexArray& va, const IndexBuffer& ib, Ref<Material> material)
    : material(material), m_va(va), m_ib(ib) {}

StaticMesh::StaticMesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, Ref<Material> material)
    : material(material)
    , m_va(vertices)
    , m_ib(indices) {}