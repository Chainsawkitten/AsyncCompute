#include <GL/glew.h>
#include "OpenGLRenderer.hpp"
#include "../Base/Window.hpp"
#include <iostream>

OpenGLRenderer::OpenGLRenderer(Window & window) {

    // Setup glfw window.
    window.setWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    window.setWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    window.setWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    window.setWindowHint(GLFW_RESIZABLE, GL_FALSE);
    std::string windowTitle = "OpenGL";
    window.createWindow(windowTitle);

    glfwMakeContextCurrent(window.getWindow());

    // Setup GLEW.
    if(glewInit() != GLEW_OK){
        std::cout << "Glew initialized" << std::endl;
    }

    // Setup viewport.
    int width, height;

    glfwGetFramebufferSize(window.getWindow(), &width, &height);

    glViewport(0, 0, width, height);

    // Setup shaders.
    shader = new OpenGLShader();
}

OpenGLRenderer::~OpenGLRenderer() {
    
}

void OpenGLRenderer::setTexture(const char* textureData, unsigned int dataLength) {
    
}

void OpenGLRenderer::render(){
    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}
