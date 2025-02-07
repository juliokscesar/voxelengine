#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <tuple>
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

struct GlobalStatistics {
  uint32_t glCalls = 0;
};
static GlobalStatistics g_stats;
#define GLCALL(F) g_stats.glCalls++;F

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
