#include "engine.h"

void GLLibManager::initGLFW() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, VE_GL_VERSION_MAJOR);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, VE_GL_VERSION_MINOR);

    glfwWindowHint(GLFW_FOCUS_ON_SHOW, GLFW_TRUE);
    isGLFWInit = true;
}

void GLLibManager::initGLAD() {
    if (!isCtxSet)
        return;
    isGLADInit = (bool)gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
}    

void GLLibManager::terminate() {
    if (isGLFWInit)
        glfwTerminate();
    isGLFWInit = false;
}
