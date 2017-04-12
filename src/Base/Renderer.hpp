#pragma once
#include "Window.hpp"

/// Controls rendering functionality.
class Renderer {
    public:
        /// The type of backend used to implement the renderer.
        enum class BACKEND{
            OpenGL, ///< OpenGL backend.
            Vulkan ///< Vulkan backend.
        };
        
        /// Destructor.
        virtual ~Renderer();
        
        /// Create a new renderer.
        /**
         * @param backend The backend to use for rendering.
         * @return The created renderer.
         */
        static Renderer* makeRenderer(BACKEND backend, Window & createdWindow);

        /// Static window variable.
        static Window* window;
};
