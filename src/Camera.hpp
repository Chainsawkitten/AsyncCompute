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
         * @param screenize The size of the screen.
         * @return The view projection matrix.
         */
        glm::mat4 getViewProjectionMatrix(const glm::vec2& screenSize) const;
        
    private:
        glm::mat4 getCameraOrientation() const;
        glm::mat4 getProjection(const glm::vec2& screenSize) const;
        
        glm::vec3 position = glm::vec3(0.f, 0.f, 10.f);
        glm::vec3 rotation = glm::vec3(0.f, 0.f, 0.f);
        
        float fieldOfView = 45.f;
        float zNear = 0.5f;
        float zFar = 100.f;
};
