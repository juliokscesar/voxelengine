#pragma once

#include "core/base.h"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;

    Vertex(const glm::vec3& pos, const glm::vec3& normal, const glm::vec2& texCoords)
        : position(pos), normal(normal), texCoords(texCoords) {}
};

struct VertexArray {
    uint32_t vboID;
    uint32_t vaoID;
    uint32_t nVertices;
    
    VertexArray(const std::vector<Vertex>& verticesData);
    VertexArray(const VertexArray& other);
    VertexArray(VertexArray&& other);

    VertexArray() = delete;

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

    void bind();
    void unbind();    
};

struct IndexBuffer {
    uint32_t eboID;
    uint32_t nIndices;

    IndexBuffer(const std::vector<uint32_t>& indices);
    IndexBuffer(const IndexBuffer& other);
    IndexBuffer(IndexBuffer&& other);

    IndexBuffer() = delete;

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

    void bind();
    void unbind();
};