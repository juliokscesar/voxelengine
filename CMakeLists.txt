cmake_minimum_required(VERSION 3.28)

project(notanengine)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})

set(EXT_DIR ${PROJECT_SOURCE_DIR}/ext)
set(SRC_DIR ${PROJECT_SOURCE_DIR}/src)

add_definitions(-DNAE_GL_VERSION_MAJOR=4 -DNAE_GL_VERSION_MINOR=6)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(${EXT_DIR}/glfw)

set(SRC_FILES
    ${EXT_DIR}/glad/src/glad.c

    ${SRC_DIR}/core/engine.cpp
    ${SRC_DIR}/core/renderer.cpp
    ${SRC_DIR}/core/resource_manager.cpp
    ${SRC_DIR}/core/window.cpp

    ${SRC_DIR}/main.cpp
)

add_executable(${PROJECT_NAME} ${SRC_FILES})

target_include_directories(${PROJECT_NAME} PUBLIC
    ${EXT_DIR}/glfw/include
    ${EXT_DIR}/glad/include

    ${SRC_DIR}
)

target_link_libraries(${PROJECT_NAME} PUBLIC
    OpenGL::GL
    glfw
)
