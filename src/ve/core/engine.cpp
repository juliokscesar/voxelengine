#include "engine.h"

#include "input.h"
#include "logging.h"
#include "shader.h"

Engine::Engine(const WindowProps& winProps)
    : m_isUp(false), m_isRunning(false), m_window(winProps) {

    g_glLibMgr = std::make_unique<GLLibManager>();
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

    g_glLibMgr->initGLFW();
    if (!g_glLibMgr->isGLFWInit) {
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
    g_glLibMgr->isCtxSet = true;
    g_glLibMgr->initGLAD();
    if (!g_glLibMgr->isGLADInit) {
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
    return true;
}

bool Engine::run() {
    if (m_isRunning)
        return true;
    if (!m_isUp && !startup())
        return false;

    float vertices[] = {
        -0.5f, -0.5f, 0.0f, // lb
         0.5f, -0.5f, 0.0f, // rb
         0.5f,  0.5f, 0.0f, // ru
        -0.5f,  0.5f, 0.0f  // lu
    };
    uint32_t indices[] = {
        3, 0, 1,
        1, 2, 3,
    };
    const size_t nind = sizeof(indices) / sizeof(indices[0]);

    uint32_t vao{};
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    uint32_t vbo{};
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    uint32_t ebo{};
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // pos vert attrb
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    Shader baseShader(
        "D:\\Dev\\voxelengine\\shaders\\basic.vert",
        "D:\\Dev\\voxelengine\\shaders\\basic.frag",
        true
    );
    if (!baseShader.isUsable()) {
        grflog::fatal("Unable to use base shader");
        return false;
    }
    baseShader.use();

    m_renderer.setClearColor(0.5f, 0.1f, 0.8f);
    
    // enter main loop
    m_isRunning = true;
    while (!m_window.shouldClose()) {
        m_renderer.frameStart();

        if (input::isKeyPressed(GLFW_KEY_ESCAPE))
            m_window.notifyClose();
        if (input::isKeyPressed(GLFW_KEY_G))
            grflog::info("GL calls: {}", g_glLibMgr->glCalls);

        // here goes the main rendering
        // lets separate the rendering in layers (one for the game, one for the ui, any post processing in between etc)

        baseShader.use();
        glBindVertexArray(vao);
        GLCALL(glDrawElements(GL_TRIANGLES, nind, GL_UNSIGNED_INT, 0));

        m_renderer.frameEnd();
        glfwPollEvents();
    }

    return true;
}

void Engine::shutdown() {
    m_isUp = false;
    m_isRunning = false;

    g_glLibMgr->terminate();
}
