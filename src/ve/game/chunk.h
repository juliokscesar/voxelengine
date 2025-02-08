#pragma once

#include "core/base.h"
#include "block.h"

class Chunk {
public:
    Chunk(uint32_t dim, uint32_t height, BlockType fillBlock);
    ~Chunk();

    void init(BlockType fillBlock);
    void update(float deltaTime);

public:
    TransformComponent transform;
    std::vector<Block> blocks;

    uint32_t dim;
    uint32_t height;

private:
    bool m_hasUpdated = false;
};


