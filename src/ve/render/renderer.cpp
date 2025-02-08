#include "renderer.h"

#include <glm/gtc/matrix_transform.hpp>

#include "core/logging.h"
#include "components/camera.h"

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
    m_aspectRatio = (float)width / (float)height;
    updateProjection();
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

void Renderer::setRenderCamera(Ref<Camera> camera) {
    m_renderCamera = camera;
    updateProjection();
}

void Renderer::updateProjection() {
    float fov = 45.0f, pNear = 0.0f, pFar = 100.0f;
    if (m_renderCamera)
        fov = m_renderCamera->fov, pNear = m_renderCamera->pNear, pFar = m_renderCamera->pFar;
    m_projectionMatrix = glm::perspective(fov, m_aspectRatio, pNear, pFar);
}

void Renderer::draw(const VertexArray& va, const IndexBuffer& ib) const {
    va.bind();
    GLCALL(glDrawElements(GL_TRIANGLES, ib.nIndices, GL_UNSIGNED_INT, 0));
}

void Renderer::draw(const StaticMesh &mesh) const {
    for (const auto& submesh : mesh.subMeshes) {
        useMaterialShader(submesh.material->shader, submesh.material);
        draw(submesh.va, submesh.ib);
    }
}

void Renderer::draw(const Entity& entity) const {
    if (!entity.isVisible())
        return;
    if (!m_renderCamera) {
        grflog::warn("Renderer: calling draw(entity) requires a render camera to be set");
        return;
    }
    for (const auto& submesh : entity.mesh.subMeshes) {
        Ref<Shader> shader = (submesh.material->shader != nullptr) ? submesh.material->shader : m_defaultShader;
        useMaterialShader(shader, submesh.material);
        shader->setUniformMat4("model", entity.transform.getTransformMatrix());
        shader->setUniformMat4("view", m_renderCamera->getViewMatrix());
        shader->setUniformMat4("projection", m_projectionMatrix);
        draw(submesh.va, submesh.ib);
    }
}

void Renderer::draw(const std::vector<Entity>& entities) const {
    for (const auto& entity : entities)
        draw(entity);
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

void Renderer::setCulling(bool enabled, GLenum face, GLenum order) {
    if (enabled) {
        glEnable(GL_CULL_FACE);
        glCullFace(face);
        glFrontFace(order);
    }
    else
        glDisable(GL_CULL_FACE);
}

void Renderer::useMaterialShader(Ref<Shader> shader, Ref<Material> material) const {
    if (!shader)
        shader = m_defaultShader;

    if (!shader || !material)
        return;

    shader->use();
    const std::string uMatFmt = "material.";
    shader->setUniformVec3(uMatFmt+"ambient", material->ambient);
    shader->setUniformFloat(uMatFmt+"tilingFactor", material->tilingFactor);
    // set diffuse maps in even numbered units
    size_t size = static_cast<uint32_t>(material->diffuse.size());
    std::string nameFmt = uMatFmt+".diffuseMaps[";
    for (uint32_t i = 0, u = 0; i < size; i++) {
        Ref<Texture2D> diffMap = material->diffuse[i];
        diffMap->setUnit(u);
        diffMap->bind();

        shader->setUniformUInt(nameFmt + std::to_string(i) + "]", u);

        u += 2;
    }

    // only set specular and shininess if using light
    if (material->useLighting) {
        shader->setUniformFloat(uMatFmt+"shininess", material->shininess);

        size = static_cast<uint32_t>(material->specular.size());
        nameFmt = uMatFmt+".specularMaps[";
        // specular goes in odd numbered units
        for (uint32_t i = 0, u = 1; i < size; i++) {
            Ref<Texture2D> specMap = material->specular[i];
            specMap->setUnit(u);
            specMap->bind();

            shader->setUniformUInt(nameFmt + std::to_string(i) + "]", u);

            u += 2;
        }
    }
}
