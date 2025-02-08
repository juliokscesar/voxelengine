#pragma once

#include "core/base.h"
#include "core/entity.h"

class Chunk {
public:
    Chunk(uint32_t dim, uint32_t height);
    ~Chunk();

    void init(uint32_t dim, uint32_t height);

public:
    TransformComponent transform;
    std::vector<Entity> cubes;

private:
    uint32_t m_dim;
    uint32_t m_height;
};
