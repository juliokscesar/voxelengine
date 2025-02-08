#include "chunk.h"

Chunk::Chunk(uint32_t dim, uint32_t height) {
    init(dim, height);
}

Chunk::~Chunk() {
}

void Chunk::init(uint32_t dim, uint32_t height) {
    m_dim = dim;
    m_height = height;
    cubes.reserve(dim * dim * height);

    StaticMesh cubeMesh(PrimitiveMesh::cube(glm::u8vec4(0, 255, 0, 255)));

}
