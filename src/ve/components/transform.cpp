#include "transform.h"

#include <glm/gtc/matrix_transform.hpp>

TransformComponent::TransformComponent(const glm::vec3& pos,
                                       const glm::vec3& rot,
                                       const glm::vec3& scale)
    : m_position(pos), m_rotation(rot), m_scale(scale), m_transMatrix(1.0f) {
    updateTransformMatrix();
}

void TransformComponent::setPosition(const glm::vec3& pos) { m_position = pos; }

void TransformComponent::setPosition(float x, float y, float z) {
    m_position = glm::vec3(x, y, z);
}

void TransformComponent::translate(const glm::vec3& pos) {
    setPosition(m_position + pos);
}

void TransformComponent::setRotation(const glm::vec3& rot) { m_rotation = rot; }

void TransformComponent::setRotation(float x, float y, float z) {
    m_rotation = glm::vec3(x, y, z);
}

void TransformComponent::rotate(float deg, const glm::vec3& axis) {
    // TODO: implement rotation around other axis
    // this works if axis is (1,0,0) (0,1,0) or (0,0,1)
    // but i have no idea if it works around other axis
    glm::vec3 newRot = glm::normalize(axis) * deg;
    setRotation(newRot);
}

void TransformComponent::setScale(const glm::vec3& scale) { m_scale = scale; }

void TransformComponent::setScale(float x, float y, float z) {
    m_scale = glm::vec3(x, y, z);
}

void TransformComponent::scaleBy(float factor) { m_scale = glm::vec3(factor); }

void TransformComponent::update() { updateTransformMatrix(); }

void TransformComponent::updateTransformMatrix() {
    m_transMatrix = glm::mat4(1.0f);

    // Translation (relative to origin (0,0,0))
    m_transMatrix = glm::translate(m_transMatrix, m_position);

    // Rotation
    // TODO: QUATERNIONS
    float angleX = glm::radians(m_rotation.x);
    float angleY = glm::radians(m_rotation.y);
    float angleZ = glm::radians(m_rotation.z);

    // x-axis
    if (angleX)
        m_transMatrix =
            glm::rotate(m_transMatrix, angleX, glm::vec3(1.0f, 0.0f, 0.0f));
    // y-axis
    if (angleY)
        m_transMatrix =
            glm::rotate(m_transMatrix, angleY, glm::vec3(0.0f, 1.0f, 0.0f));
    // z-axis
    if (angleZ)
        m_transMatrix =
            glm::rotate(m_transMatrix, angleZ, glm::vec3(0.0f, 0.0f, 1.0f));

    // Scale
    m_transMatrix = glm::scale(m_transMatrix, m_scale);
}