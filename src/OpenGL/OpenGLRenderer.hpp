#pragma once
#include "OpenGLShader.hpp"
#include "../Base/Renderer.hpp"

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
        GLfloat vertices[32] = {
            // Positions          // Colors           // Texture Coords
             0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // Top Right
             0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // Bottom Right
            -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // Bottom Left
            -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // Top Left
        };

        /// Triangle indices. TEMPORARY.
        GLuint indices[6] = {  // Note that we start from 0!
            0, 1, 3, // First Triangle
            1, 2, 3  // Second Triangle
        };

        /// SSBO.
        GLuint SSBO;

        /// Texture
        GLuint texture;

};
