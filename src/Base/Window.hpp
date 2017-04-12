#pragma once
#include <GLFW/glfw3.h>
#include <string>

/// Class that controls and creates a window.
class Window {
    public:
        /// Constructor
        Window();

        /// Destructor
        ~Window();

        /// Sets a GLFW window hint. Has to be called before creating window.
        /**
         * @param field The field that will be set.
         * @param value The value to set.
         */
        void setWindowHint(int field, int value);

        /// Creates a GLFW window.
        /**
         * @param windowTitle The title of the glfw window.
         * @return Created GLFW window.
         */
        GLFWwindow* createWindow(const std::string& windowTitle);

        /// GLFW window.
        GLFWwindow* window;

        /// Gets window width.
        /**
         * @return
         */
        int getWidth();

        /// Gets window height.
        /**
         * @return
         */
        int getHeight();

    private:
        int width = 800;
        int height = 600;
};
