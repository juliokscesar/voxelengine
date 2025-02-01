#include "engine.h"

#include "log.h"
#include "input.h"

void GLLibManager::initGLFW() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, VE_GL_VERSION_MAJOR);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, VE_GL_VERSION_MINOR);

    glfwWindowHint(GLFW_FOCUS_ON_SHOW, GLFW_TRUE);
    isGLFWInit = true;
}

void GLLibManager::initGLAD() {
    if (!isCtxSet)
        return;
    isGLADInit = (bool)gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
}    

void GLLibManager::terminate() {
    if (isGLFWInit)
        glfwTerminate();
    isGLFWInit = false;
}

Engine::Engine(const WindowProps& winProps)
    : m_isUp(false), m_isRunning(false), m_window(winProps) {
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

void resizeCallback(GLFWwindow* window, int width, int height) {
    VE_UNUSED(window);

    g_resizeProperties.hasChanged = true;
    g_resizeProperties.width = width;
    g_resizeProperties.height = height;
}

bool Engine::startup() {
    m_glLibMgr.initGLFW();
    if (!m_glLibMgr.isGLFWInit) {
        grflog::fatal("Unable to start GLFW");
        shutdown();
        return false;
    }

    if (!m_window.init()) {
        grflog::fatal("Unable to create window");
        shutdown();
        return false;
    }

    m_renderer.setCtx(m_window.glfwWindow());

    m_glLibMgr.initGLAD();
    if (!m_glLibMgr.isGLADInit) {
        grflog::fatal("Unable to start GLAD");
        shutdown();
        return false;
    }

    const WindowProps& wp = m_window.getWinProps();
    m_renderer.setVSyncEnabled(wp.enableVSync);
    m_renderer.resizeViewport(wp.width, wp.height);
    glfwSetFramebufferSizeCallback(m_window.glfwWindow(), resizeCallback);

    input::registerCallbacks(m_window.glfwWindow());
    input::setCursorMode(GLFW_CURSOR_DISABLED);

    m_isUp = true;  
}

bool Engine::run() {
    if (!startup())
        return false;

    // enter main loop
    m_isRunning = true;
    while (!m_window.shouldClose()) {
        m_renderer.frameStart();

        // here goes the main rendering
        // lets separate the rendering in layers (one for the game, one for the ui, any post processing in between etc)        

        m_renderer.frameEnd();
        glfwPollEvents();
    }

    return true;
}

void Engine::shutdown() {
    m_isUp = false;
    m_isRunning = false;

    m_glLibMgr.terminate();
}
