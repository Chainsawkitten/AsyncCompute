#pragma once
#include <GLFW/glfw3.h>
#include <string>

class Window {
    public:
        /// setWindowHint Sets a GLFW window hint. Has to be called before creating window.
        /**
         * @param field The field that will be set.
         * @param value The value to set.
         */
        void setWindowHint(int field, int value);

        /// createWindow Creates a GLFW window.
        /**
         * @param windowTitle The title of the glfw window.
         * @return Created GLFW window.
         */
        GLFWwindow* createWindow(std::string windowTitle);

    private:
        GLFWwindow* window;
        int width = 800;
        int height = 600;
};
