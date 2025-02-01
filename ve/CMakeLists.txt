cmake_minimum_required(VERSION 3.28)

project(vecore)

set(SRC_DIR ${PROJECT_SOURCE_DIR}/src)

add_definitions(-DVE_GL_VERSION_MAJOR=4 -DVE_GL_VERSION_MINOR=6)

set(SRC_FILES
    ${EXT_DIR}/glad/src/glad.c
    ${EXT_DIR}/griffinLogCpp/src/griffinLog/griffinLog.cpp

    ${SRC_DIR}/core/engine.cpp
    ${SRC_DIR}/core/renderer.cpp
    ${SRC_DIR}/core/resource_manager.cpp
    ${SRC_DIR}/core/window.cpp
    ${SRC_DIR}/core/application.cpp
    ${SRC_DIR}/core/input.cpp
)

add_library(${PROJECT_NAME} ${SRC_FILES})
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