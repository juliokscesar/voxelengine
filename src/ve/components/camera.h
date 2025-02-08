#pragma once

#include "transform.h"

enum class CameraDirection : uint32_t {
    FORWARD = 0,
    RIGHT   = 1,
    UP      = 2
};

class Camera {
public:
    Camera();
    ~Camera();

    void update(/*float deltaTime*/);

    void move(const glm::vec3& offset);
    void move(CameraDirection dir, float speed);

    void setPitch(float pitch);
    void addPitch(float delta);

    void setYaw(float yaw);
    void addYaw(float delta);

    const glm::mat4& getViewMatrix() const;

public:
    TransformComponent transform;
    
    float fov = 45.0f;
    float pNear = 0.0f;
    float pFar = 100.0f;

private:
    bool m_updated;
    glm::vec3 m_worldUp;
    glm::vec3 m_up;
    glm::vec3 m_front;
    glm::vec3 m_right;

    float m_yaw     = 0.0f;
    float m_pitch   = 0.0f;

    glm::mat4 m_lookAtMatrix;
};
