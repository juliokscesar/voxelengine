#include "camera.h"

#include <glm/gtc/matrix_transform.hpp>

#include "core/logging.h"

#define MAX_PITCH 89.0f
#define MAX_YAW 360.0f

Camera::Camera()
    : m_updated(false)
    , m_worldUp(0.0f, 1.0f, 0.0f)
    , m_up(0.0f, 1.0f, 0.0f)
    , m_front(0.0f, 0.0f, 1.0f)
    , m_right(1.0f, 0.0f, 0.0f)
    , m_yaw(-90.0f)
    , m_pitch(0.0f) {
    
    m_lookAtMatrix = glm::lookAt(transform.getPosition(), transform.getPosition() + m_front, m_worldUp);
}

Camera::~Camera() {
}

void Camera::update(/*float deltaTime*/) {
    if (!m_updated)
        return;

    transform.update();

    glm::vec3 direction(
        /* x */ glm::cos(glm::radians(m_yaw)) * glm::cos(glm::radians(m_pitch)),
        /* y */ glm::sin(glm::radians(m_pitch)),
        /* z */ glm::sin(glm::radians(m_yaw)) * glm::cos(glm::radians(m_pitch))
    );
    m_front = glm::normalize(direction);
    m_right = glm::normalize(glm::cross(m_front, m_worldUp));
    m_up = glm::normalize(glm::cross(m_front, m_right));
    m_lookAtMatrix = glm::lookAt(transform.getPosition(), transform.getPosition() + m_front, m_worldUp);
    
    m_updated = false;
}

void Camera::move(const glm::vec3& offset) {
    transform.translate(offset);
    m_updated = true;
}

void Camera::move(CameraDirection dir, float speed) {
    static const glm::vec3* s_directions[] = {
        &m_front, &m_right, &m_worldUp
    };
    move(*(s_directions[static_cast<uint32_t>(dir)]) * speed);
}

void Camera::setPitch(float pitch) {
    m_pitch = std::clamp(pitch, -MAX_PITCH, MAX_PITCH);
    m_updated = true;
}

void Camera::addPitch(float delta) {
    setPitch(m_pitch + delta);
}

void Camera::setYaw(float yaw) {
    m_yaw = yaw;
    if (yaw >= MAX_YAW || m_yaw <= -MAX_YAW)
        m_yaw = 0.0f;

    m_updated = true;
}

void Camera::addYaw(float delta) {
    setYaw(m_yaw + delta);
}

const glm::mat4& Camera::getViewMatrix() const {
    return m_lookAtMatrix;
}