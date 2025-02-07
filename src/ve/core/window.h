#pragma once

#include "base.h"

struct WindowProps {
    uint32_t width;
    uint32_t height;

    std::string title;

    bool enableVSync;
    bool enableFullscreen;

    WindowProps(uint32_t width, uint32_t height, const std::string& title, bool enableVSync, bool enableFullscreen)
        : width(width), height(height), title(title), enableVSync(enableVSync), enableFullscreen(enableFullscreen) {}
};

// TODO: fullscreen
class Window {
public:
    Window(const WindowProps& winProps);
    Window(uint32_t width, uint32_t height, const std::string& title, bool enableVSync, bool enableFullscreen);
    ~Window();

    inline const WindowProps& getWinProps() const { return m_winProps; }
    
    bool init();
    bool shouldClose();
    void notifyClose();
    GLFWwindow* glfwWindow();

    inline void setWidth(uint32_t width)    { m_winProps.width  = width; }
    inline void setHeight(uint32_t height)  { m_winProps.height = height; }
    inline void setDimensions(uint32_t width, uint32_t height) { m_winProps.width = width; m_winProps.height = height; }

private:
    WindowProps m_winProps;
    GLFWwindow* m_glfwWindow;
};
