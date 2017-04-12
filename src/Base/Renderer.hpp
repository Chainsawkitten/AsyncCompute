#pragma once

/// Controls rendering functionality.
class Renderer {
    public:
        /// The type of backend used to implement the renderer.
        enum class BACKEND{
            OpenGL, ///< OpenGL backend.
            Vulkan ///< Vulkan backend.
        };
        
        /// Create a new renderer.
        /**
         * @param backend The backend to use for rendering.
         * @return The created renderer.
         */
        static Renderer* makeRenderer(BACKEND backend);
};
