#include "input.h"

#define ACTION 0
#define MODS 1

#include <unordered_map>

namespace input {

struct KeyState {
    uint32_t action;
    uint32_t mods;
    bool isPressed;
};
static std::unordered_map<uint32_t, KeyState> g_keyStateMap;

// gets current state from map if key is registered
// else registers it to the map
KeyState& keyCurrState(uint32_t key) {
    // key is registered
    if (auto state = g_keyStateMap.find(key); state != g_keyStateMap.end())
        return state->second;
    
    // key not registered
    g_keyStateMap[key] = KeyState();
    return g_keyStateMap[key];
}

struct MouseTracker {
    float lastX = 0.0f;
    float lastY = 0.0f;
    float currX = 0.0f;
    float currY = 0.0f;

    float xOffset = 0.0f;
    float yOffset = 0.0f;

    bool isFirst = true;

    float scrollOffset = 0.0f;
};
static MouseTracker g_mouseTracker;

void keyInputCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (auto state = g_keyStateMap.find(key); state == g_keyStateMap.end())
        g_keyStateMap[key] = KeyState();

    g_keyStateMap[key].action = action;
    g_keyStateMap[key].mods = mods;

    KeyState& keyState = keyCurrState(key);
    keyState.action = action;
    keyState.mods = mods;

    if (action == GLFW_PRESS)
        keyState.isPressed = true;
    else if (action == GLFW_RELEASE)
        keyState.isPressed = false;
}

void mouseMoveCallback(GLFWwindow* window, double xpos, double ypos) {
    if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_NORMAL)
        return;

    if (g_mouseTracker.isFirst) {
        g_mouseTracker.lastX = static_cast<float>(xpos);
        g_mouseTracker.lastY = static_cast<float>(ypos);
        g_mouseTracker.currX = g_mouseTracker.lastX;
        g_mouseTracker.currY = g_mouseTracker.lastY;

        g_mouseTracker.xOffset = 0.0f;
        g_mouseTracker.yOffset = 0.0f;

        g_mouseTracker.isFirst = false;

        return;
    }

    g_mouseTracker.lastX = g_mouseTracker.currX;
    g_mouseTracker.lastY = g_mouseTracker.currY;

    g_mouseTracker.currX = static_cast<float>(xpos);
    g_mouseTracker.currY = static_cast<float>(ypos);

    g_mouseTracker.xOffset = g_mouseTracker.currX - g_mouseTracker.lastX;
    g_mouseTracker.yOffset = g_mouseTracker.currY - g_mouseTracker.lastY;
}

void mouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    g_mouseTracker.scrollOffset = static_cast<float>(yoffset);
}

uint32_t getKeyLastAction(int key) {
    return keyCurrState(key).action;
}

uint32_t getKeyLastMods(int key) {
    return keyCurrState(key).mods;
}

bool isKeyPressed(int key) {
    return keyCurrState(key).isPressed;
}

static GLFWwindow* g_ctxWindow;
void registerCallbacks(GLFWwindow* window) {
    glfwSetKeyCallback(window, keyInputCallback);
    glfwSetCursorPosCallback(window, mouseMoveCallback);
    glfwSetScrollCallback(window, mouseScrollCallback);
    g_ctxWindow = window;
}

float getMouseXOffset() {
    if (g_mouseTracker.isFirst)
        return 0.0f;

    float offset = g_mouseTracker.xOffset;
    g_mouseTracker.xOffset = 0.0f;
    return offset;
}

float getMouseYOffset() {
    if (g_mouseTracker.isFirst)
        return 0.0f;

    float offset = g_mouseTracker.yOffset;
    g_mouseTracker.yOffset = 0.0f;
    return offset;
}

float getMouseScroll() {
    float scroll = g_mouseTracker.scrollOffset;
    g_mouseTracker.scrollOffset = 0.0f;
    return scroll;
}

void setCursorMode(int mode) {
    // GLFW_CURSOR_NORMAL: default. Regular cursor used and no limitations to motion
    // GLFW_CURSOR_DISABLED: hides and locks the cursor to the current window
    // GLFW_CURSOR_HIDDEN: hides the cursor but it still behaves normally
    glfwSetInputMode(g_ctxWindow, GLFW_CURSOR, mode);
}

int getCursorMode() {
    return glfwGetInputMode(g_ctxWindow, GLFW_CURSOR);
}

}