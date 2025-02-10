#include "engine.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "input.h"
#include "logging.h"
#include "render/shader.h"
#include "components/transform.h"
#include "components/camera.h"
#include "game/chunk.h"

Engine::Engine(Ref<Game> game)
    : m_isUp(false)
    , m_isRunning(false)
    , m_glLibMgr(createRef<GLLibManager>())
    , m_renderer(createRef<Renderer>())
    , m_window(createRef<Window>(game->getWinProps()))
    , m_game(game) {

    m_game->setRenderer(m_renderer);
    m_game->setResourceManager(m_resMgr);
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

static void resizeCallback(GLFWwindow*, int width, int height) {
    g_resizeProperties.hasChanged = true;
    g_resizeProperties.width = width;
    g_resizeProperties.height = height;
}

bool Engine::start() {
    if (m_isUp)
        return true;
    if (!m_game) {
        grflog::fatal("The game must be set in order to start the engine.");
        return false;
    }

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
    m_renderer->setCulling(true, GL_BACK, GL_CCW);
    m_renderer->setClearColor(0.2f, 0.2f, 0.2f);

    input::registerCallbacks(m_window->glfwWindow());
    input::setCursorMode(GLFW_CURSOR_DISABLED);

    m_isUp = true;  
    m_game->onStart();
    return true;
}

bool Engine::run() {
    if (m_isRunning)
        return true;
    if (!m_isUp && !start())
        return false;

    Ref<Shader> shader = m_resMgr->loadShader("D:\\Dev\\voxelengine\\shaders\\material.vert",
                                              "D:\\Dev\\voxelengine\\shaders\\material.frag");
    m_renderer->setDefaultShader(shader);
    m_resMgr->loadGPUData();
    GLCHECK("");

    // enter main loop
    m_isRunning = true;
    auto lastTime = hrclock::now();
    while (!m_window->shouldClose()) {
        auto currTime = hrclock::now();
        float deltaTime = std::chrono::duration<float>(currTime - lastTime).count();
        lastTime = currTime;

        if (g_resizeProperties.hasChanged) {
            m_renderer->resizeViewport(g_resizeProperties.width, g_resizeProperties.height);
            m_window->setDimensions(g_resizeProperties.width, g_resizeProperties.height);
            m_game->getWinProps() = m_window->getWinProps();
            g_resizeProperties.hasChanged = false;
        }

        m_renderer->frameStart();

#ifndef RELEASE
        if (input::isKeyPressed(GLFW_KEY_ESCAPE))
            m_window->notifyClose();
        if (input::isKeyPressed(GLFW_KEY_G))
            grflog::info("GL calls: {}", g_stats.glCalls);
        if (input::isKeyPressed(GLFW_KEY_P))
            m_renderer->toggleWireframe();
#endif

        m_game->onUpdate(deltaTime);

        // Camera controls
        // if (input::isKeyPressed(GLFW_KEY_LEFT_SHIFT))
        //     speed *= 1.5f;
        // if (input::isKeyPressed(GLFW_KEY_W))
        //     camera->move(CameraDirection::FORWARD, speed);
        // if (input::isKeyPressed(GLFW_KEY_S))
        //     camera->move(CameraDirection::FORWARD, -speed);
        // if (input::isKeyPressed(GLFW_KEY_D))
        //     camera->move(CameraDirection::RIGHT, speed);
        // if (input::isKeyPressed(GLFW_KEY_A))
        //     camera->move(CameraDirection::RIGHT, -speed);
        // if (input::isKeyPressed(GLFW_KEY_SPACE))
        //     camera->move(CameraDirection::UP, speed);
        // if (input::isKeyPressed(GLFW_KEY_LEFT_CONTROL))
        //     camera->move(CameraDirection::UP, -speed);
        

        // float mouse = input::getMouseXOffset();
        // if (mouse)
        //     camera->addYaw(mouse * camSens);
        // mouse = input::getMouseYOffset();
        // if (mouse)
        //     camera->addPitch(-mouse * camSens);

        // camera->update();
        // chunk.update(deltaTime);

        m_renderer->updateProjection();

        // here goes the main rendering
        // lets separate the rendering in layers (one for the game, one for the ui, any post processing in between etc)

        m_renderer->frameEnd();
        glfwPollEvents();
    }

    return true;
}

void Engine::shutdown() {
    m_isUp = false;
    m_isRunning = false;

    m_game->onShutdown();
    m_glLibMgr->terminate();
}
