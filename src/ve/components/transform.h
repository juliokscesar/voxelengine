#pragma once

#include "core/base.h"

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

/*
    A transform component contains position, scale and rotation.
    The transform matrix is stored and calculated through this.
*/
class TransformComponent {
public:
    TransformComponent()
        : m_position(0.0f), m_rotation(0.0f), m_scale(1.0f),
          m_transMatrix(1.0f) {}

    TransformComponent(const glm::vec3& pos, const glm::vec3& rot = glm::vec3(0.0f), const glm::vec3& scale = glm::vec3(1.0f));

    TransformComponent(const TransformComponent &other)
        : m_position(other.m_position), m_rotation(other.m_rotation),
          m_scale(other.m_scale), m_transMatrix(1.0f) {}

    TransformComponent &operator=(const TransformComponent &other) {
        if (this != &other) {
            this->m_position = other.m_position;
            this->m_rotation = other.m_rotation;
            this->m_scale = other.m_scale;
        }
        this->updateTransformMatrix();
        return *this;
    }

    inline glm::vec3 getPosition() const noexcept { return m_position; }
    inline glm::vec3 getRotation() const noexcept { return m_rotation; }
    inline glm::vec3 getScale() const noexcept { return m_scale; }

    inline glm::mat4 getTransformMatrix() const noexcept {
        return m_transMatrix;
    }

    void setPosition(const glm::vec3 &pos);
    void setPosition(float x, float y, float z);
    void translate(const glm::vec3 &pos);
    inline glm::vec3 &getPositionRef() noexcept { return m_position; }

    void setRotation(const glm::vec3 &rot);
    void setRotation(float x, float y, float z);
    void rotate(float deg, const glm::vec3 &axis);
    inline glm::vec3 &getRotationRef() noexcept { return m_rotation; }

    void setScale(const glm::vec3 &scale);
    void setScale(float x, float y, float z);
    void scaleBy(float factor);
    inline glm::vec3 &getScaleRef() noexcept { return m_scale; }

    void update();

private:
    glm::vec3 m_position;
    glm::vec3 m_rotation;
    glm::vec3 m_scale;

    glm::mat4 m_transMatrix;

    void updateTransformMatrix();
};