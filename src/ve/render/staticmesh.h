#pragma once

#include "core/base.h"

#include "material.h"
#include "vertex.h"

class StaticMesh {
    StaticMesh(const VertexArray& va, const IndexBuffer& ib, Ref<Material> material);
    StaticMesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, Ref<Material> material);

    inline const VertexArray& getVertexArray() const { return m_va; }
    inline const IndexBuffer& getIndexBuffer() const { return m_ib; }
    inline Ref<Material> getMaterial() { return material; }

public:
    Ref<Material> material;

private:
    VertexArray m_va;
    IndexBuffer m_ib;
};
