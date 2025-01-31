#include "engine.h"

namespace engine {
    void GLLibManager::initGLFW() {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, NAE_GL_VERSION_MAJOR);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, NAE_GL_VERSION_MINOR);

        glfwWindowHint(GLFW_FOCUS_ON_SHOW, GLFW_TRUE);
        isGLFWInit = true;
    }

    void GLLibManager::initGLAD() {
        isGLADInit = (bool)gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    }    
}
