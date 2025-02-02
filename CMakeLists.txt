cmake_minimum_required(VERSION 3.28)

project(voxelengine)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE DEBUG)
endif()

if(NOT MSVC)
    set(CMAKE_CXX_FLAGS_DEBUG "-g")
    set(CMAKE_CXX_FLAGS_RELEASE "-O2")
endif()

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})

set(EXT_DIR ${PROJECT_SOURCE_DIR}/ext)
set(SRC_DIR ${PROJECT_SOURCE_DIR}/src/ve)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(${EXT_DIR}/glfw)

set(GLM_BUILD_TESTS OFF)
set(BUILD_SHARED_LIBS OFF)
add_subdirectory(${EXT_DIR}/glm)

add_definitions(-DVE_GL_VERSION_MAJOR=4 -DVE_GL_VERSION_MINOR=6)
set(SRC_FILES
    ${EXT_DIR}/glad/src/glad.c
    ${EXT_DIR}/griffinLogCpp/src/griffinLog/griffinLog.cpp

    ${SRC_DIR}/core/engine.cpp
    ${SRC_DIR}/core/renderer.cpp
    ${SRC_DIR}/core/resource_manager.cpp
    ${SRC_DIR}/core/window.cpp
    ${SRC_DIR}/core/input.cpp

    ${SRC_DIR}/main.cpp
)

add_executable(${PROJECT_NAME} ${SRC_FILES})
target_include_directories(${PROJECT_NAME} PUBLIC
    ${EXT_DIR}/glfw/include
    ${EXT_DIR}/glad/include
    ${EXT_DIR}/glm
    ${EXT_DIR}/griffinLogCpp/src

    ${SRC_DIR}
)
target_link_libraries(${PROJECT_NAME} PUBLIC
    OpenGL::GL
    glfw
    glm::glm
)
