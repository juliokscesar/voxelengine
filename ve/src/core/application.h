#pragma once

#include "engine.h"

class Application {
public:
    Application(uint32_t width, uint32_t height, const std::string& title, bool enableVSync, bool enableFullscreen);
    Application(const WindowProps& winProps);
    ~Application();

    void run();
private:
    Engine m_engine;
};
