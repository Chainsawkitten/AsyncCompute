#include "Window.hpp"

GLFWwindow* Window::createWindow(std::string windowTitle){
    return window = glfwCreateWindow(width, height, windowTitle.c_str(), nullptr, nullptr);
}

void Window::setWindowHint(int field, int value){
    glfwWindowHint(field, value);
}
