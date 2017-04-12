#pragma once

#include "../Base/Renderer.hpp"
#include "../Base/Window.hpp"

/// OpenGL implementation of the renderer.
class OpenGLRenderer : public Renderer {
    public:
        /// Create new OpenGL renderer.
        OpenGLRenderer(Window & window);
        
        /// Destructor.
        ~OpenGLRenderer() final;
};
