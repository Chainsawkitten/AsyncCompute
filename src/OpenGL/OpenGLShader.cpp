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

    }
}
