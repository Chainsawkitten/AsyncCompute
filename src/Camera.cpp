#include "Camera.hpp"

#include <glm/gtc/matrix_transform.hpp>

Camera::Camera() {
    
}

Camera::~Camera() {
    
}

glm::mat4 Camera::GetViewProjectionMatrix(const glm::vec2& screenSize) const {
    glm::mat4 viewMat(GetCameraOrientation() * glm::translate(glm::mat4(), -position));
    return GetProjection(screenSize) * viewMat;
}

glm::mat4 Camera::GetCameraOrientation() const {
    glm::mat4 orientation;
    orientation = glm::rotate(orientation, glm::radians(rotation.z), glm::vec3(0.f, 0.f, 1.f));
    orientation = glm::rotate(orientation, glm::radians(rotation.y), glm::vec3(1.f, 0.f, 0.f));
    return glm::rotate(orientation, glm::radians(rotation.x), glm::vec3(0.f, 1.f, 0.f));
}

glm::mat4 Camera::GetProjection(const glm::vec2& screenSize) const {
    return glm::perspective(glm::radians(fieldOfView), screenSize.x/screenSize.y, zNear, zFar);
}
