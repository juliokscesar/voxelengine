#include "entity.h"

Entity::Entity()
    : transform()
    , mesh()
    , m_isVisible(true) {}

Entity::Entity(const StaticMesh &mesh)
    : mesh(mesh) {}

Entity::~Entity() {
}

void Entity::update(float deltaTime) {
    transform.update();
}
