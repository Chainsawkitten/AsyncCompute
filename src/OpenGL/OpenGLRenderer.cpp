#include "OpenGLRenderer.hpp"
#include <gl/glew.h>

OpenGLRenderer::OpenGLRenderer(Window window) {
    // Setup glfw window.
    window.setWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    window.setWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    window.setWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    window.setWindowHint(GLFW_RESIZABLE, GL_FALSE);
    std::string windowTitle = "OpenGL";
    window.createWindow(windowTitle);
}

OpenGLRenderer::~OpenGLRenderer() {
    
}
