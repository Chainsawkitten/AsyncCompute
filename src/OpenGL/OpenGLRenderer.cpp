#include <GL/glew.h>
#include "OpenGLRenderer.hpp"


OpenGLRenderer::OpenGLRenderer(Window & window) {
    // Setup glfw window.
    window.setWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    window.setWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    window.setWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    window.setWindowHint(GLFW_RESIZABLE, GL_FALSE);
    std::string windowTitle = "OpenGL";
    window.createWindow(windowTitle);

    // Setup GLEW
    glewExperimental = true;
    if(glewInit() != GLEW_OK){

    }

    // Setup viewport
    int width, height;

    glfwGetFramebufferSize(window.getWindow(), &width, &height);

    glViewport(0, 0, width, height);
}

OpenGLRenderer::~OpenGLRenderer() {
    
}
