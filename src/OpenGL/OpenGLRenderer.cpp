#include <GL/glew.h>
#include "OpenGLRenderer.hpp"
#include "../Base/Window.hpp"
#include "Particle.png.hpp"
#include <stb_image.h>
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

    // Setup SSBO
    glGenBuffers(1, &SSBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, SSBO);
    glBufferData(GL_SHADER_STORAGE_BUFFER, 32, vertices, GL_STATIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, SSBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

    // Create and bind texture.
    setTexture(PARTICLE_PNG, PARTICLE_PNG_LENGTH);
}

OpenGLRenderer::~OpenGLRenderer() {

}

void OpenGLRenderer::setTexture(const char* textureData, unsigned int dataLength) {
    int width, height, channels;

    // Generate texture on the GPU.
    glGenTextures(1, &texture);

    // Bind texture.
    glBindTexture(GL_TEXTURE_2D, texture);

    // Use STB to load image.
    stbi_uc* pixels = stbi_load_from_memory(reinterpret_cast<const unsigned char*>(textureData), dataLength, &width, &height, &channels, 0);

    // Transfer image to GPU.
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

    // Generate mipmap.
    glGenerateMipmap(GL_TEXTURE_2D);

    // Unbind texture.
    glBindTexture(GL_TEXTURE_2D, 0);

    // Clean up.
    stbi_image_free(pixels);
}

void OpenGLRenderer::render(){
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Use shader program.
    glUseProgram(shader->shaderProgram);

    // Setup texture for rendering.
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(glGetUniformLocation(shader->shaderProgram,"texture"), 0);


}
