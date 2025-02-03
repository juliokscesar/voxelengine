#pragma once

#include "core/base.h"

#include <glm/vec3.hpp>

#include "vertex.h"
#include "staticmesh.h"

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

    void draw(const VertexArray& va, const IndexBuffer& ib) const;
    void draw(const StaticMesh& mesh) const;

    void setClearColor(const glm::vec3& rgb);
    void setClearColor(float r, float g, float b);

    void setDepthTest(bool enabled, GLenum func = GL_LESS);
    void setBlending(bool enabled, GLenum func = GL_ONE_MINUS_SRC_ALPHA);
    // TODO: how am i going to implement all these enable and disable options?
    void setStencilTest(bool enabled, GLenum func = GL_NOTEQUAL); // NOT IMPLEMENTED!!!

private:
    GLFWwindow* m_ctxWindow;

    GLbitfield m_bufferBits;
    glm::vec3 m_clearBufferColor;
};
