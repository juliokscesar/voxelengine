#include "logging.h"

#include "base.h"

void _checkGLError(const char *file, int line, const std::string &msg) {
    GLenum errorCode;
    std::string error;
    while ((errorCode = glGetError()) != GL_NO_ERROR) {
        switch (errorCode) {
        case GL_INVALID_ENUM:
            error = "INVALID_ENUM";
            break;

        case GL_INVALID_VALUE:
            error = "INVALID_VALUE";
            break;

        case GL_INVALID_OPERATION:
            error = "INVALID_OPERATION";
            break;

        case GL_INVALID_FRAMEBUFFER_OPERATION:
            error = "INVALID_FRAMEBUFFER_OPERATION";
            break;

        case GL_OUT_OF_MEMORY:
            error = "OUT_OF_MEMORY";
            break;
        }

        grflog::critical("(OpenGL ERROR) {} | {} ({}): {}", error, file, line, msg);
    }
}