#pragma once

#include "base.h"

#include "renderer.h"
#include "window.h"
#include "resource_manager.h"

struct GLLibManager {
    bool isGLFWInit;
    void initGLFW();
    bool isCtxSet;

    bool isGLADInit;
    void initGLAD();

    void terminate();
};


class Engine {
public:
    Engine();
    ~Engine();

    bool run();
    void shutdown();

private:
    GLLibManager m_glLibMgr;
    Renderer m_renderer;
    Window m_window;
    ResourceManager m_resMgr;
};
