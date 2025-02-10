#pragma once

#include "core/base.h"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
    uint32_t  materialID;

    Vertex()
        : position(0), normal(0), texCoords(0), materialID(0) {}

    Vertex(const glm::vec3& pos, const glm::vec3& normal, const glm::vec2& texCoords, uint32_t materialID)
        : position(pos), normal(normal), texCoords(texCoords), materialID(0) {}
};

struct VertexArray {
    uint32_t vboID;
    uint32_t vaoID;
    uint32_t nVertices;
    
    VertexArray()
        : vboID(0), vaoID(0), nVertices(0) {}

    VertexArray(const std::vector<Vertex>& verticesData);
    VertexArray(const VertexArray& other);
    VertexArray(VertexArray&& other);

    VertexArray& operator=(const VertexArray& other) {
        if (this != &other) {
            this->vaoID      = other.vaoID;
            this->vboID      = other.vboID;
            this->nVertices  = other.nVertices;
        }
        return *this;
    }

    VertexArray& operator=(VertexArray&& other) {
        if (this != &other) {
            this->vaoID     = std::move(other.vaoID);
            this->vboID     = std::move(other.vboID);
            this->nVertices = std::move(other.nVertices);
        }
        return *this;
    }

    void create();
    void init(const std::vector<Vertex>& verticesData);
    void renderData(const std::vector<Vertex>& verticesData);
    void bind() const;
    void unbind() const;
};

struct IndexBuffer {
    uint32_t eboID;
    uint32_t nIndices;

    IndexBuffer()
        : eboID(0), nIndices(0) {}

    IndexBuffer(const std::vector<uint32_t>& indices);
    IndexBuffer(const IndexBuffer& other);
    IndexBuffer(IndexBuffer&& other);

    IndexBuffer& operator=(const IndexBuffer& other) {
        if (this != &other) {
            this->eboID     = other.eboID;
            this->nIndices  = other.nIndices;
        }
        return *this;
    }

    IndexBuffer& operator=(IndexBuffer&& other) {
        if (this != &other) {
            this->eboID     = std::move(other.eboID);
            this->nIndices  = std::move(other.nIndices);
        }
        return *this;
    }

    void init(const std::vector<uint32_t>& indices);
    void bind() const;
    void unbind() const;
};