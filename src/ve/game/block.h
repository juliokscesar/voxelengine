#pragma once

#include "core/entity.h"

enum class BlockType {
    AIR = 0,
    GROUND = 1,
};

class Block : public Entity {
public:
    Block(const glm::vec3& initialPos, BlockType type, const StaticMesh& mesh);
    ~Block();

    void update(float deltaTime) override;

    BlockType type;
};

Block blockBuilder(BlockType type, const glm::vec3& initPos);