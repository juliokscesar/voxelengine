#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <string>
#include <filesystem>
#include <memory>
#include <utility>

namespace stdfs = std::filesystem;

#ifdef _WIN32
    #include <windows.h>
#endif // _WIN32

#ifndef VE_GL_VERSION_MAJOR
    #define VE_GL_VERSION_MAJOR 4
#endif
#ifndef VE_GL_VERSION_MINOR
    #define VE_GL_VERSION_MINOR 6
#endif

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define VE_UNUSED(param) ((void)param)

struct GLLibManager {
    bool isGLFWInit = false;
    bool isCtxSet = false;
    bool isGLADInit = false;
    uint32_t glCalls = 0;

    inline void initGLFW() {
        glfwInit();

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, VE_GL_VERSION_MAJOR);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, VE_GL_VERSION_MINOR);

        glfwWindowHint(GLFW_FOCUS_ON_SHOW, GLFW_TRUE);
        isGLFWInit = true;
    }

    inline void initGLAD() {
      if (!isCtxSet)
        return;
      isGLADInit = (bool)gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    }

    inline void terminate() {
      if (isGLFWInit)
        glfwTerminate();
      isGLFWInit = false;
    }
};
static std::unique_ptr<GLLibManager> g_glLibMgr = std::make_unique<GLLibManager>();
#define GLCALL(F) g_glLibMgr->glCalls++;F

template<typename T>
using Scope = std::unique_ptr<T>;
template<typename T, typename ...Args>
constexpr Scope<T> createScope(Args&& ... args) {
    return std::make_unique<T>(T(std::forward<Args>(args)...));
}

template<typename T>
using Ref = std::shared_ptr<T>;
template<typename T, typename ...Args>
constexpr Ref<T> createRef(Args&& ... args) {
    return std::make_shared<T>(T(std::forward<Args>(args)...));
}
