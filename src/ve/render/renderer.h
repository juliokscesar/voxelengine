#pragma once

#include "core/base.h"

#include <glm/vec3.hpp>

#include "vertex.h"
#include "staticmesh.h"
#include "core/entity.h"
#include "components/camera.h"

class Renderer {
public:
    Renderer();
    ~Renderer();

    // stuff related to the window
    void setCtx(GLFWwindow* window);
    void setVSyncEnabled(bool enabled);
    void resizeViewport(uint32_t width, uint32_t height);

    void frameStart();
    void clearBuffers();
    void frameEnd();

    void setRenderCamera(Ref<Camera> camera);
    inline Ref<Camera> getRenderCamera() { return m_renderCamera; }

    void updateProjection();

    void draw(const VertexArray& va, const IndexBuffer& ib) const;
    void draw(const StaticMesh& mesh) const;
    void draw(const Entity& entity) const;
    void draw(const std::vector<Entity>& entities) const;

    void setClearColor(const glm::vec3& rgb);
    void setClearColor(float r, float g, float b);

    void setDepthTest(bool enabled, GLenum func = GL_LESS);
    void setBlending(bool enabled, GLenum func = GL_ONE_MINUS_SRC_ALPHA);
    // TODO: how am i going to implement all these enable and disable options?
    void setStencilTest(bool enabled, GLenum func = GL_NOTEQUAL); // NOT IMPLEMENTED!!!
    void setCulling(bool enabled, GLenum face = GL_BACK, GLenum order = GL_CCW);

    inline void setDefaultShader(Ref<Shader> shader) { m_defaultShader = shader; }
    Ref<Shader> getDefaultShader() { return m_defaultShader; }

    void useMaterialShader(Ref<Shader> shader, Ref<Material> material) const;

    void toggleWireframe();

private:
    GLFWwindow* m_ctxWindow;

    GLbitfield m_bufferBits;
    glm::vec3 m_clearBufferColor;

    Ref<Shader> m_defaultShader;
    Ref<Camera> m_renderCamera;

    float m_aspectRatio = 1.0f;
    glm::mat4 m_projectionMatrix;

    GLenum m_glPolygonMode = GL_FILL;
};
