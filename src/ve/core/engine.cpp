#include "engine.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "input.h"
#include "logging.h"
#include "render/shader.h"
#include "components/transform.h"
#include "components/camera.h"

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
    m_renderer->setCulling(true, GL_BACK, GL_CCW);

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
    Ref<Texture2D> woodTex = m_resMgr->loadTexture("D:\\Dev\\voxelengine\\assets\\textures\\LargeTreeBark.jpg");
    Ref<Material> woodMat = createRef<Material>();
    woodMat->diffuse.push_back(woodTex);
    woodMat->useLighting = false;
    woodMat->ambient = glm::vec3(0.4f);
    woodMat->shininess = 0.1f;
    woodMat->tilingFactor = 1.0f;

    StaticMesh cube = PrimitiveMesh::cube();
    cube.subMeshes[0].material = woodMat;
    cube.subMeshes[0].material->shader = shader;
    cube.subMeshes[0].material->useLighting = false;
    TransformComponent transform;

    GLCHECK("");

    Camera camera;
    float camSpeed = 1.0f;
    float camSens = 1.0f;
    camera.transform.setPosition(0.0f, 0.0f, 3.0f);
    camera.update();

    float ar = (float)m_window->getWinProps().width/(float)m_window->getWinProps().height;
    glm::mat4 proj = glm::perspective(camera.fov, ar, 0.1f, 100.0f);

    m_renderer->setClearColor(0.3f, 0.3f, 0.3f);
    
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
            ar = static_cast<float>(g_resizeProperties.width) / static_cast<float>(g_resizeProperties.height);
            g_resizeProperties.hasChanged = false;
        }

        float speed = camSpeed * deltaTime;

        m_renderer->frameStart();

        if (input::isKeyPressed(GLFW_KEY_ESCAPE))
            m_window->notifyClose();
        if (input::isKeyPressed(GLFW_KEY_G))
            grflog::info("GL calls: {}", g_stats.glCalls);

        // Camera controls
        if (input::isKeyPressed(GLFW_KEY_W))
            camera.move(CameraDirection::FORWARD, speed);
        if (input::isKeyPressed(GLFW_KEY_S))
            camera.move(CameraDirection::FORWARD, -speed);
        if (input::isKeyPressed(GLFW_KEY_D))
            camera.move(CameraDirection::RIGHT, speed);
        if (input::isKeyPressed(GLFW_KEY_A))
            camera.move(CameraDirection::RIGHT, -speed);
        if (input::isKeyPressed(GLFW_KEY_SPACE))
            camera.move(CameraDirection::UP, speed);
        if (input::isKeyPressed(GLFW_KEY_LEFT_SHIFT))
            camera.move(CameraDirection::UP, -speed);

        float mouse = input::getMouseXOffset();
        if (mouse)
            camera.addYaw(mouse * camSens);
        mouse = input::getMouseYOffset();
        if (mouse)
            camera.addPitch(-mouse * camSens);

        camera.update();
        proj = glm::perspective(camera.fov, ar, 0.1f, 100.0f);

        // here goes the main rendering
        // lets separate the rendering in layers (one for the game, one for the ui, any post processing in between etc)
        shader->use();
        shader->setUniformMat4("model", transform.getTransformMatrix());
        shader->setUniformMat4("view", camera.getViewMatrix());
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
