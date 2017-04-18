#include "OpenGLShader.hpp"
#include "default.vert.hpp"

OpenGLShader::OpenGLShader(){
    GLchar infoLog[512];
    GLint success;
    vertexShaderHandle = glCreateShader(GL_VERTEX_SHADER);

    GLint length = DEFAULT_VERT_LENGTH;
    const GLchar* source = DEFAULT_VERT;

    // Set shader source.
    glShaderSource(vertexShaderHandle, 1, &source, &length);

    // Compile shader.
    glCompileShader(vertexShaderHandle);

    glGetShaderiv(vertexShaderHandle, GL_COMPILE_STATUS, &success);

    // Get compile output.
    if(!success){
        glGetShaderInfoLog(vertexShaderHandle, 512, nullptr, infoLog);

    }
}
