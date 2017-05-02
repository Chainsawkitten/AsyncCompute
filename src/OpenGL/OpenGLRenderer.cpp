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
    window.setWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
    window.setWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    window.setWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    std::string windowTitle = "OpenGL";
    window.createWindow(windowTitle);

    glfwMakeContextCurrent(window.getWindow());

    // Setup viewport.
    int width, height;

    glfwGetFramebufferSize(window.getWindow(), &width, &height);

    glViewport(0, 0, width, height);

    glewExperimental = GL_TRUE;

    // Setup GLEW.
    if(glewInit() != GLEW_OK){
        std::cout << "Glew initialized" << std::endl;
    }

    // Setup shaders.
    shader = new OpenGLShader();

    glDebugMessageCallback(glDebugOutput, nullptr);
    GLuint unusedIds = 0;
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, &unusedIds, true);

    glGenBuffers(1, &dummyVBO);
    glBindBuffer(GL_ARRAY_BUFFER, dummyVBO);

    // Setup SSBO
    glGenBuffers(1, &SSBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, SSBO);
    glBufferData(GL_SHADER_STORAGE_BUFFER, 36, vertices, GL_STATIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, SSBO);
    // Unbind SSBO
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
    glUniform1i(glGetUniformLocation(shader->shaderProgram, "texture"), 0);

    // Draw.
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void APIENTRY glDebugOutput(GLenum source,
                            GLenum type,
                            GLuint id,
                            GLenum severity,
                            GLsizei length,
                            const GLchar* message,
                            const void* userParam)
{
    // ignore non-significant error/warning codes
    if(id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

    std::cout << "---------------" << std::endl;
    std::cout << "Debug message (" << id << "): " <<  message << std::endl;

    switch (source)
    {
        case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
        case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
        case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
    } std::cout << std::endl;

    switch (type)
    {
        case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break;
        case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
        case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
        case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
        case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
        case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
        case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
    } std::cout << std::endl;

    switch (severity)
    {
        case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
        case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
        case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
        case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
    } std::cout << std::endl;
    std::cout << std::endl;
}
