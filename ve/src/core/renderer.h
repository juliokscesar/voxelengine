#pragma once

#include "base.h"

class Renderer {
public:
    Renderer();
    ~Renderer();

    // stuff related to the window
    void setCtx(GLFWwindow* window);
    void setVSyncEnabled(bool enabled);
    void resizeViewport(uint32_t width, uint32_t height);
};
