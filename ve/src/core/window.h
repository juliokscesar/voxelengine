#pragma once

#include <string>

struct WindowProps {
    uint32_t width;
    uint32_t height;

    std::string title;

    bool enableVSync;
    bool enableFullscreen;
};

class Window {
public:
    Window(const WindowProps& winProps);
    Window(uint32_t width, uint32_t height, const std::string& title, bool enableVSync, bool enableFullscreen);
    ~Window();

private:
    bool init();
    

private:
    WindowProps m_winProps;
};
