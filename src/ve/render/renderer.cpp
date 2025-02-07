#include "renderer.h"

#include "core/logging.h"

Renderer::Renderer()
    : m_ctxWindow(nullptr), m_bufferBits(GL_COLOR_BUFFER_BIT) {
}

Renderer::~Renderer(){
}

void Renderer::setCtx(GLFWwindow* window) {
    glfwMakeContextCurrent(window);
    m_ctxWindow = window;
}

void Renderer::setVSyncEnabled(bool enabled) {
    glfwSwapInterval(enabled);
}

void Renderer::resizeViewport(uint32_t width, uint32_t height) {
    glViewport(0, 0, width, height);
}

void Renderer::frameStart() {
    clearBuffers();
}

void Renderer::clearBuffers() {
    glClearColor(m_clearBufferColor.r, m_clearBufferColor.g, m_clearBufferColor.b, 1.0f);
    glClear(m_bufferBits);
}

void Renderer::frameEnd() {
    glfwSwapBuffers(m_ctxWindow);
}

void Renderer::draw(const VertexArray &va, const IndexBuffer &ib) const {
    va.bind();
    GLCALL(glDrawElements(GL_TRIANGLES, ib.nIndices, GL_UNSIGNED_INT, 0));
}

void Renderer::draw(const StaticMesh &mesh) const {
    for (const auto& submesh : mesh.subMeshes()) {
        submesh.material->enableMaterialShader();
        draw(submesh.va, submesh.ib);
    }
}

void Renderer::setClearColor(const glm::vec3& rgb) {
    m_clearBufferColor = rgb;
}

void Renderer::setClearColor(float r, float g, float b) {
    m_clearBufferColor.r = r;
    m_clearBufferColor.g = g;
    m_clearBufferColor.b = b;
}

void Renderer::setDepthTest(bool enabled, GLenum func) {
    if (enabled) {
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(func);
        m_bufferBits |= GL_DEPTH_BUFFER_BIT;
    }
    else {
        glDisable(GL_DEPTH_TEST);
        if (m_bufferBits & GL_DEPTH_BUFFER_BIT)
            m_bufferBits &= ~GL_DEPTH_BUFFER_BIT;
    }
}

void Renderer::setBlending(bool enabled, GLenum func) {
    if (enabled) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, func);
    }
    else
        glDisable(GL_BLEND);
}
