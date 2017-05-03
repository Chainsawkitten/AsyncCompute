#include "Camera.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>

Camera::Camera() {
    
}

Camera::~Camera() {
    
}

glm::mat4 Camera::getViewProjectionMatrix(const glm::vec2& screenSize) const {
    glm::mat4 viewMat(getCameraOrientation() * glm::translate(glm::mat4(), -position));
    return getProjection(screenSize) * viewMat;
}

const glm::vec3& Camera::getPosition() const {
    return position;
}

glm::vec3 Camera::getUp() const {
    return glm::vec3(glm::inverse(getCameraOrientation()) * glm::vec4(0.f, 1.f, 0.f, 1.f));
}

glm::mat4 Camera::getCameraOrientation() const {
    glm::mat4 orientation;
    orientation = glm::rotate(orientation, glm::radians(rotation.z), glm::vec3(0.f, 0.f, 1.f));
    orientation = glm::rotate(orientation, glm::radians(rotation.y), glm::vec3(1.f, 0.f, 0.f));
    return glm::rotate(orientation, glm::radians(rotation.x), glm::vec3(0.f, 1.f, 0.f));
}

glm::mat4 Camera::getProjection(const glm::vec2& screenSize) const {
    return glm::perspective(glm::radians(fieldOfView), screenSize.x/screenSize.y, zNear, zFar);
}
