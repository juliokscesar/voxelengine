#include "window.h"

Window::Window(const WindowProps& winProps)
    : m_winProps(winProps) {
    this->init();
}

Window::Window(uint32_t width, uint32_t height, const std::string& title, bool enableVSync, bool enableFullscreen)
    : m_winProps(width, height, title, enableVSync, enableFullscreen) {
}

Window::~Window() {
    if (m_glfwWindow)
        glfwDestroyWindow(m_glfwWindow);
}

bool Window::init() {
    m_glfwWindow = glfwCreateWindow(
        m_winProps.width,
        m_winProps.height,
        m_winProps.title.c_str(),
        nullptr,
        nullptr
    );
    return (m_glfwWindow != nullptr);
}
