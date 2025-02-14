#pragma once

#include "core/entity.h"

enum class BlockType {
    AIR = 0,
    GROUND = 1,
};

class Block {
public:
    Block(BlockType type);
    ~Block();

    void setType(BlockType type);
    bool isSolid() const;

    BlockType type;
};

// Block blockBuilder(BlockType type, const glm::vec3& initPos);