#include "engine.h"

#include "input.h"
#include "logging.h"

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

static void resizeCallback(GLFWwindow* window, int width, int height) {
    VE_UNUSED(window);

    g_resizeProperties.hasChanged = true;
    g_resizeProperties.width = width;
    g_resizeProperties.height = height;
}

bool Engine::startup() {
    if (m_isUp)
        return true;

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
    return true;
}

bool Engine::run() {
    if (m_isRunning)
        return true;
    if (!m_isUp && !startup())
        return false;

    // Triangle
    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f,
    };

    uint32_t vao{};
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    uint32_t vbo{};
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // pos vert attrb
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    // TODO: shader, resource manager and file handler
    std::ifstream sFile("D:\\Dev\\voxelengine\\shaders\\basic.vert", std::ios::binary);
    if (!sFile.is_open())
        grflog::critical("Unable to open vert file");
    std::string vertSrcStr = std::string(
        std::istreambuf_iterator<char>(sFile), std::istreambuf_iterator<char>()
    ).c_str();
    std::cout << vertSrcStr << std::endl;
    sFile.close();
    sFile.open("D:\\Dev\\voxelengine\\shaders\\basic.frag", std::ios::binary);
    if (!sFile.is_open())
        grflog::critical("Unable to open frag file");
    std::string fragSrcStr = std::string(
        std::istreambuf_iterator<char>(sFile), std::istreambuf_iterator<char>()
    ).c_str();
    std::cout << fragSrcStr << std::endl;

    int success = 0;
    char infoLog[512];

    uint32_t vertShader = glCreateShader(GL_VERTEX_SHADER);
    const char* vertSrc = vertSrcStr.c_str();
    glShaderSource(vertShader, 1, &vertSrc, nullptr);
    glCompileShader(vertShader);

    glGetShaderiv(vertShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertShader, sizeof(infoLog), nullptr, infoLog);
        grflog::fatal("Error compiling vertex shader: ", infoLog);
        return false;
    }

    uint32_t fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    const char* fragSrc = fragSrcStr.c_str();
    glShaderSource(fragShader, 1, &fragSrc, nullptr);
    glCompileShader(fragShader);

    glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragShader, sizeof(infoLog), nullptr, infoLog);
        grflog::fatal("Error compiling fragment shader: {}", infoLog);
        return false;
    }

    uint32_t baseShader = glCreateProgram();
    glAttachShader(baseShader, vertShader);
    glAttachShader(baseShader, fragShader);
    glLinkProgram(baseShader);
    
    glValidateProgram(baseShader);
    glGetProgramiv(baseShader, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(baseShader, sizeof(infoLog), nullptr, infoLog);
        grflog::fatal("Failed to link shader program: {}", infoLog);
        return false;
    }

    glDeleteShader(vertShader);
    glDeleteShader(fragShader);

    // enter main loop
    m_isRunning = true;
    while (!m_window.shouldClose()) {
        m_renderer.frameStart();

        // here goes the main rendering
        // lets separate the rendering in layers (one for the game, one for the ui, any post processing in between etc)

        glUseProgram(baseShader);
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);

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
