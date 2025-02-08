#include "entity.h"

Entity::Entity()
    : transform(), mesh(), m_isVisible(false) {}

Entity::Entity(const StaticMesh &mesh)
    : mesh(mesh), m_isVisible(true) {}

Entity::Entity(const glm::vec3& initPos, const StaticMesh& mesh)
    : transform(initPos), mesh(mesh), m_isVisible(true) {
}

Entity::~Entity() {
}

void Entity::update(float deltaTime) {
    transform.update();
}
