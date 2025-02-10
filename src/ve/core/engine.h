#pragma once

#include "base.h"

#include "render/renderer.h"
#include "window.h"
#include "resource_manager.h"

#include "game/game.h"

struct GLLibManager {
    bool isGLFWInit = false;
    bool isCtxSet = false;
    bool isGLADInit = false;

    inline void initGLFW() {
        glfwInit();

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, VE_GL_VERSION_MAJOR);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, VE_GL_VERSION_MINOR);

        glfwWindowHint(GLFW_FOCUS_ON_SHOW, GLFW_TRUE);
        isGLFWInit = true;
    }

    inline void initGLAD() {
      if (!isCtxSet)
        return;
      isGLADInit = (bool)gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    }

    inline void terminate() {
      if (isGLFWInit)
        glfwTerminate();
      isGLFWInit = false;
    }
};

class Engine {
public:
    Engine(Ref<Game> game);
    ~Engine();

    bool start();
    bool run();
    void shutdown();

private:
    bool m_isUp;
    bool m_isRunning;

    Ref<GLLibManager> m_glLibMgr;
    Ref<Renderer> m_renderer;
    Ref<Window> m_window;
    Ref<ResourceManager> m_resMgr;

    Ref<Game> m_game;
};
