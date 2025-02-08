#include "chunk.h"

#include "core/logging.h"

Chunk::Chunk(uint32_t dim, uint32_t height, BlockType fillBlock)
    : dim(dim), height(height) {
    blocks.reserve(dim * dim * height);
    init(fillBlock);
}

Chunk::~Chunk() {
}

void Chunk::init(BlockType fillBlock) {
    const uint32_t blockN = dim * dim * height;
    float offset = 1.01f;
    for (uint32_t i = 0; i < blockN; i++) {
        const uint32_t x = (i % dim) % (dim * dim); // east
        const uint32_t y = i / (dim * dim); // height
        const uint32_t z = (i / dim) % dim; // north

        const glm::vec3 pos = transform.getPosition() + glm::vec3(x * offset, y * offset, z * offset);

        blocks.push_back(blockBuilder(
            fillBlock,
            pos
        ));
    }

    m_hasUpdated = true;
    update(0.0f);
}

void Chunk::update(float deltaTime) {
    if (!m_hasUpdated)
        return;
    
    transform.update();
    for (auto& block : blocks) {
        block.update(deltaTime);
    }

    m_hasUpdated = false;
}
