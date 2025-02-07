#include "engine.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "input.h"
#include "logging.h"
#include "render/shader.h"
#include "components/transform.h"

Engine::Engine(const WindowProps& winProps)
    : m_isUp(false) 
    , m_isRunning(false)
    , m_glLibMgr(std::make_unique<GLLibManager>())
    , m_renderer(std::make_unique<Renderer>())
    , m_window(std::make_unique<Window>(winProps))
    , m_resMgr(std::make_unique<ResourceManager>()) {
}

Engine::~Engine() {
    shutdown();
}

struct ResizeProperties {
    bool hasChanged = false;
    uint32_t width = 0;
    uint32_t height = 0;
};
static ResizeProperties g_resizeProperties;

static void resizeCallback(GLFWwindow* window, int width, int height) {
    VE_UNUSED(window);

    g_resizeProperties.hasChanged = true;
    g_resizeProperties.width = width;
    g_resizeProperties.height = height;
}

bool Engine::startup() {
    if (m_isUp)
        return true;

    m_glLibMgr->initGLFW();
    if (!m_glLibMgr->isGLFWInit) {
        grflog::fatal("Unable to start GLFW");
        shutdown();
        return false;
    }

    if (!m_window->init()) {
        grflog::fatal("Unable to create window");
        shutdown();
        return false;
    }

    m_renderer->setCtx(m_window->glfwWindow());
    m_glLibMgr->isCtxSet = true;
    m_glLibMgr->initGLAD();
    if (!m_glLibMgr->isGLADInit) {
        grflog::fatal("Unable to start GLAD");
        shutdown();
        return false;
    }
    GLCHECK("");

    const WindowProps& wp = m_window->getWinProps();
    m_renderer->setVSyncEnabled(wp.enableVSync);
    m_renderer->resizeViewport(wp.width, wp.height);
    glfwSetFramebufferSizeCallback(m_window->glfwWindow(), resizeCallback);

    m_renderer->setDepthTest(true);

    input::registerCallbacks(m_window->glfwWindow());
    input::setCursorMode(GLFW_CURSOR_DISABLED);

    m_isUp = true;  
    return true;
}

bool Engine::run() {
    if (m_isRunning)
        return true;
    if (!m_isUp && !startup())
        return false;

    Ref<Shader> shader = m_resMgr->loadShader("D:\\Dev\\voxelengine\\shaders\\material.vert", "D:\\Dev\\voxelengine\\shaders\\material.frag");
    StaticMesh cube = PrimitiveMesh::cube();
    cube.subMeshes()[0].material->shader = shader;
    cube.subMeshes()[0].material->useLighting = false;
    TransformComponent transform;

    const float ar = (float)m_window->getWinProps().width/(float)m_window->getWinProps().height;
    glm::mat4 proj = glm::perspective(glm::radians(45.0f), ar, 0.1f, 100.0f);

    glm::mat4 view(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

    m_renderer->setClearColor(0.3f, 0.3f, 0.3f);
    
    // enter main loop
    m_isRunning = true;
    while (!m_window->shouldClose()) {
        m_renderer->frameStart();

        if (input::isKeyPressed(GLFW_KEY_ESCAPE))
            m_window->notifyClose();
        if (input::isKeyPressed(GLFW_KEY_G))
            grflog::info("GL calls: {}", g_stats.glCalls);

        // here goes the main rendering
        // lets separate the rendering in layers (one for the game, one for the ui, any post processing in between etc)
        shader->use();
        shader->setUniformMat4("model", transform.getTransformMatrix());
        shader->setUniformMat4("view", view);
        shader->setUniformMat4("projection", proj);
        m_renderer->draw(cube);
        GLCHECK("");

        m_renderer->frameEnd();
        glfwPollEvents();
    }

    return true;
}

void Engine::shutdown() {
    m_isUp = false;
    m_isRunning = false;

    m_glLibMgr->terminate();
}
