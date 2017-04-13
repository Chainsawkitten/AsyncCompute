#include "Window.hpp"

Window::Window() {
    glfwInit();
}

Window::~Window() {
    glfwTerminate();
}

void Window::setWindowHint(int field, int value) {
    glfwWindowHint(field, value);
}

GLFWwindow* Window::createWindow(const std::string & windowTitle) {
    return window = glfwCreateWindow(width, height, windowTitle.c_str(), nullptr, nullptr);
}

GLFWwindow* Window::getWindow() const {
    return window;
}

int Window::getWidth() const {
    return width;
}

int Window::getHeight() const {
    return height;
}
