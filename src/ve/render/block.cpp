#include "block.h"

// Block blockBuilder(BlockType type, const glm::vec3& initPos) {
//     static std::unordered_map<BlockType, StaticMesh> blockMeshMap({
//         {BlockType::AIR, StaticMesh()},
//         {BlockType::GROUND, PrimitiveMesh::cube(glm::u8vec4(0, 200, 0, 255))},
//     });

//     Block block(initPos, type, blockMeshMap[type]);
//     if (type == BlockType::AIR)
//         block.setIsVisible(false);
    
//     return block;
// }

// Block::Block(const glm::vec3& initialPos, BlockType type,
//              const StaticMesh& mesh)
//     : Entity(initialPos, mesh), type(type) {
//     if (type  == BlockType::AIR)
//         setIsVisible(false);
// }

// Block::~Block() {}

// void Block::update(float deltaTime) {
//     Entity::update(deltaTime);
// }

Block::Block(BlockType type)
    : type(type) {}

Block::~Block() {}

void Block::setType(BlockType type) {
    this->type = type;
}

bool Block::isSolid() const { 
    return this->type != BlockType::AIR; 
}
