#include "OpenGLShader.hpp"
#include "default.vert.hpp"
#include "default.frag.hpp"
#include <iostream>

OpenGLShader::OpenGLShader(){
    GLchar infoLog[512];
    GLint success;
    GLint length;
    const GLchar* source;

    // Vertex shader.
    // Create vertex shader on the GPU.
    vertexShaderHandle = glCreateShader(GL_VERTEX_SHADER);

    // Set vertex shader source.
    source = DEFAULT_VERT;
    length = DEFAULT_VERT_LENGTH;

    glShaderSource(vertexShaderHandle, 1, &source, &length);

    // Compile vertex shader.
    glCompileShader(vertexShaderHandle);

    glGetShaderiv(vertexShaderHandle, GL_COMPILE_STATUS, &success);

    // Get compile output.
    if(!success){
        glGetShaderInfoLog(vertexShaderHandle, 512, nullptr, infoLog);
        std::cout << "Vertex shader compilation failed : " << infoLog << std::endl;
    }

    // Fragment shader.
    // Create fragment shader on the GPU.
    source = DEFAULT_FRAG;
    length = DEFAULT_FRAG_LENGTH;

    fragmentShaderHandle = glCreateShader(GL_FRAGMENT_SHADER);

    // Set fragment shader source.
    glShaderSource(fragmentShaderHandle, 1, &source, &length);

    // Compile fragment shader.
    glCompileShader(fragmentShaderHandle);

    glGetShaderiv(fragmentShaderHandle, GL_COMPILE_STATUS, &success);

    // Get compile output.
    if(!success){
        glGetShaderInfoLog(fragmentShaderHandle, 512, nullptr, infoLog);
        std::cout << "Fragment shader compilation failed : " << infoLog << std::endl;
    }

    // Create and link program.
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShaderHandle);
    glAttachShader(shaderProgram, fragmentShaderHandle);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

    // Get program log.
    if(!success) {
        glGetShaderInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cout << "Program shader compilation failed : " << infoLog << std::endl;
    }

    glDeleteShader(vertexShaderHandle);
    glDeleteShader(fragmentShaderHandle);
}
