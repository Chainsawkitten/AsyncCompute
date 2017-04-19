#pragma once

#include "../Base/Renderer.hpp"
#include "../Base/Window.hpp"

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

        /// Render image to screen.
        void render() final;
};
