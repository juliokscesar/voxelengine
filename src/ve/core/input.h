#pragma once

#include "base.h"

using glfwKeyCallbackFunc = void(*)(GLFWwindow* window, int key, int scancode, int action, int mods);

namespace input {
    void registerCallbacks(GLFWwindow* window);

    uint32_t getKeyLastAction(int key);
    uint32_t getKeyLastMods(int key);
    bool isKeyPressed(int key);

    float getMouseXOffset();
    float getMouseYOffset();
    float getMouseScroll();

    void setCursorMode(int mode);
    int getCursorMode();
};
