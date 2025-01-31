#pragma once

#include "base.h"

#include "renderer.h"
#include "window.h"

struct GLLibManager {
    bool isGLFWInit;
    void initGLFW();

    bool isGLADInit;
    void initGLAD();
};

struct EngineContext {
    GLLibManager glLibMgr;
    Renderer renderer;
    Window window;
};