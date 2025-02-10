#pragma once

#include "core/base.h"
#include "block.h"
#include "vertex.h"

class Chunk {
public:
    Chunk(uint32_t dim, uint32_t height, BlockType fillBlock);
    ~Chunk();

    void init(BlockType fillBlock);
    void update();

private:
    void updateGPUMesh();
    void checkAndHideBlocks();

    Block& blockFromRelXYZ(uint32_t x, uint32_t y, uint32_t z);
    void blockRelXYZFromIdx(uint32_t idx, uint32_t& x, uint32_t& y, uint32_t& z);

public:
    TransformComponent transform;
    std::vector<Block> blocks;

    uint32_t dim;
    uint32_t height;

private:
    bool m_hasUpdated = false;

    VertexArray m_va;
    IndexBuffer m_ib;
};


