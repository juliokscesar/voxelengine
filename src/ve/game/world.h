#pragma once

#include "core/base.h"

#include "components/camera.h"
#include "components/transform.h"
#include "core/entity.h"
#include "render/renderer.h"

class World {
public:
    World();;
    World(const glm::vec3& worldOrig, const glm::vec3& worldRotation, const glm::vec3& worldScale);
    ~World();

public:
    TransformComponent worldTransform;

private:
    Ref<Camera> m_worldCamera;

    Ref<Shader> m_defaultWorldShader;
    Ref<Renderer> m_renderer;

    std::vector<Ref<Entity>> entities;
};
