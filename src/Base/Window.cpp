#include "Window.hpp"

Window::Window() {
    glfwInit();
}

Window::~Window() {
    glfwTerminate();
}

GLFWwindow* Window::createWindow(const std::string & windowTitle) {
    return window = glfwCreateWindow(width, height, windowTitle.c_str(), nullptr, nullptr);
}

GLFWwindow* Window::getWindow(){
    return window;
}

void Window::setWindowHint(int field, int value) {
    glfwWindowHint(field, value);
}

int Window::getWidth() {
    return width;
}

int Window::getHeight() {
    return height;
}
