#pragma once
#include <GL/glew.h>

/// Load, create and compile a shader for OpenGL.
class OpenGLShader {
    public:
        /// Constructor
        OpenGLShader();

        /// Shader program
        GLuint shaderProgram;
    private:
        GLuint vertexShaderHandle;
        GLuint fragmentShaderHandle;
};
