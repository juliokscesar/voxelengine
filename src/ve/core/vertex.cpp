#include "vertex.h"

VertexArray::VertexArray(const std::vector<Vertex>& verticesData) {
    nVertices = verticesData.size();

    glGenVertexArrays(1, &vaoID);
    glBindVertexArray(vaoID);

    glGenBuffers(1, &vboID);
    glBindBuffer(GL_ARRAY_BUFFER, vboID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * nVertices, verticesData.data(), GL_STATIC_DRAW);

    constexpr size_t ATT_STRIDE = sizeof(Vertex);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, ATT_STRIDE, (void*)0); // pos vertex attrib
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, ATT_STRIDE, (void*)offsetof(Vertex, normal)); // normal vertex attrib
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, ATT_STRIDE, (void*)offsetof(Vertex, texCoords)); // tex vert attrib
    glEnableVertexAttribArray(2);
}

VertexArray::VertexArray(const VertexArray &other)
    : vaoID(other.vaoID)
    , vboID(other.vboID)
    , nVertices(other.nVertices) {}

VertexArray::VertexArray(VertexArray &&other)
    : vaoID(std::move(other.vaoID))
    , vboID(std::move(other.vboID))
    , nVertices(std::move(other.nVertices)) {}


void VertexArray::bind() {
    glBindVertexArray(vaoID);
}

void VertexArray::unbind() {
    glBindVertexArray(0);
}

IndexBuffer::IndexBuffer(const std::vector<uint32_t>& indices) {
    nIndices = indices.size();

    glGenBuffers(1, &eboID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, nIndices * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);
}

IndexBuffer::IndexBuffer(const IndexBuffer &other) 
    : eboID(other.eboID)
    , nIndices(other.nIndices) {}

IndexBuffer::IndexBuffer(IndexBuffer &&other)
    : eboID(std::move(other.eboID))
    , nIndices(std::move(other.nIndices)) {}

void IndexBuffer::bind() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboID);
}

void IndexBuffer::unbind() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
