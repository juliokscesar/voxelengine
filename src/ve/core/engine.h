#pragma once

#include "base.h"

#include "renderer.h"
#include "window.h"
#include "resource_manager.h"

class Engine {
public:
    Engine(const WindowProps& winProps);
    ~Engine();

    bool startup();
    bool run();
    void shutdown();

private:
    bool m_isUp;
    bool m_isRunning;

    Renderer m_renderer;
    Window m_window;
    ResourceManager m_resMgr;
};
