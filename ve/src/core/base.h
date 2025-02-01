#pragma once

#ifndef VE_GL_VERSION_MAJOR
    #define VE_GL_VERSION_MAJOR 4
#endif
#ifndef VE_GL_VERSION_MINOR
    #define VE_GL_VERSION_MINOR 6
#endif

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define VE_UNUSED(param) ((void)param)

#include <memory>
#include <utility>

template<typename T>
using Scope = std::unique_ptr<T>;
template<typename T, typename ...Args>
constexpr Scope<T> CreateScope(Args&& ... args) {
    return std::make_unique<T>(T(std::forward<Args>(args)...));
}

template<typename T>
using Ref = std::shared_ptr<T>;
template<typename T, typename ...Args>
constexpr Ref<T> CreateRef(Args&& ... args) {
    return std::make_shared<T>(T(std::forward<Args>(args)...));
}
