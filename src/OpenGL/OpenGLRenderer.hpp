#pragma once
#include "OpenGLShader.hpp"
#include "../Base/Renderer.hpp"
#include <GLFW/glfw3.h>

/// OpenGL implementation of the renderer.
class OpenGLRenderer : public Renderer {
    public:
        /// Create new OpenGL renderer.
        /**
         * @param window The window we will render to.
         */
        OpenGLRenderer(Window & window);

        /// Destructor.
        ~OpenGLRenderer() final;
        
        /// Set the texture to use for the particles.
        /**
         * @param textureData The texture data.
         * @param dataLength The length of the texture data.
         */
        void setTexture(const char* textureData, unsigned int dataLength) final;

        /// Render image to screen.
        void render() final;

        /// Shader
        OpenGLShader* shader;

        /// Vertices used to render texture. TEMPORARY.
        GLfloat vertices[36] = {
            // Positions                // Texture Coords
             0.5f,  0.5f, 0.0f, 1.0f,   1.0f, 1.0f,   // Top Right
             0.5f, -0.5f, 0.0f, 1.0f,   1.0f, 0.0f,   // Bottom Right
            -0.5f,  0.5f, 0.0f, 1.0f,   0.0f, 1.0f,   // Top Left
             0.5f, -0.5f, 0.0f, 1.0f,   1.0f, 0.0f,
            -0.5f, -0.5f, 0.0f, 1.0f,   0.0f, 0.0f,   // Bottom Left
            -0.5f,  0.5f, 0.0f, 1.0f,   0.0f, 1.0f
        };

        /// SSBO.
        GLuint SSBO;

        /// Texture.
        GLuint texture;

        /// Empty vertex array object.
        GLuint dummyVAO;

};

void APIENTRY glDebugOutput(GLenum source,
                            GLenum type,
                            GLuint id,
                            GLenum severity,
                            GLsizei length,
                            const GLchar* message,
                            const void* userParam);
