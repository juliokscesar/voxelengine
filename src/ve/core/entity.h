#pragma once

#include "components/transform.h"
#include "render/staticmesh.h"

class Entity {
public:
    Entity();
    Entity(const StaticMesh& mesh);
    Entity(const glm::vec3& initPos, const StaticMesh& mesh);
    ~Entity();

    virtual void update(float deltaTime);

    inline bool isVisible() const { return m_isVisible; }
    inline void setIsVisible(bool isVisible) { m_isVisible = isVisible; }

public:
    TransformComponent transform;
    StaticMesh mesh;

protected:
    bool m_isVisible;
};
