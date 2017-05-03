#pragma once

#include <glm/glm.hpp>

/// Used to navigate a scene.
class Camera {
    public:
        /// Create new camera.
        Camera();
        
        /// Destructor.
        ~Camera();
        
        /// Get the view projetion matrix.
        /**
         * @return The view projection matrix.
         */
        glm::mat4 GetViewProjectionMatrix() const;
        
    private:
        glm::vec3 position = glm::vec3(0.f, 0.f, 0.f);
        glm::vec3 rotation = glm::vec3(0.f, 0.f, 0.f);
};
