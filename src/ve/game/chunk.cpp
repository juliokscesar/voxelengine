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
    float offset = 1.00f;
    for (uint32_t i = 0; i < blockN; i++) {
        uint32_t x, y, z;
        blockRelXYZFromIdx(i, x, y, z);
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
    
    checkAndHideBlocks();
    transform.update();
    for (auto& block : blocks) {
        block.update(deltaTime);
    }

    m_hasUpdated = false;
}

void Chunk::checkAndHideBlocks() {
    // check for blocks that are inside and make them invisible
    // also check for blocks that were invisible but have to change
    const size_t blockN = dim * dim * height;
    for (uint32_t i = 0; i < blockN; i++) {
        if (blocks[i].type == BlockType::AIR) {
            blocks[i].setIsVisible(false);
            continue;
        }

        uint32_t x, y, z;
        blockRelXYZFromIdx(i, x, y, z);

        // edge blocks
        if (y == 0 || y == (height-1) || x == 0 || x == (dim - 1) || z == 0 || z == (dim - 1))
            blocks[i].setIsVisible(true);
        else if (dim >= 3) {
            // if not edge, check if any neighbor is air
            if (blockFromRelXYZ(x - 1, y, z).type == BlockType::AIR ||
                blockFromRelXYZ(x + 1, y, z).type == BlockType::AIR ||
                blockFromRelXYZ(x, y - 1, z).type == BlockType::AIR ||
                blockFromRelXYZ(x, y + 1, z).type == BlockType::AIR ||
                blockFromRelXYZ(x, y, z - 1).type == BlockType::AIR ||
                blockFromRelXYZ(x, y, z + 1).type == BlockType::AIR) {
                
                blocks[i].setIsVisible(true);
            } else
                blocks[i].setIsVisible(false);
        }
    }
}

Block& Chunk::blockFromRelXYZ(uint32_t x, uint32_t y, uint32_t z) {
    const uint32_t idx = (z * dim * dim) + (y * dim) + x;
    return blocks[idx];
}

void Chunk::blockRelXYZFromIdx(uint32_t idx, uint32_t& x, uint32_t& y, uint32_t& z) { 
    x = (idx % dim) % (dim * dim); // east
    y = idx / (dim * dim); // height
    z = (idx / dim) % dim; // north
}
