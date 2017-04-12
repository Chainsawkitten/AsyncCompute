#pragma once

#include "../Base/Renderer.hpp"

/// OpenGL implementation of the renderer.
class OpenGLRenderer : public Renderer {
    public:
        /// Create new OpenGL renderer.
        OpenGLRenderer();
        
        /// Destructor.
        ~OpenGLRenderer() final;
};
