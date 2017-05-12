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

        /// Gets GLFW window.
        /**
         * @return The window created with createWindow.
         */
        GLFWwindow* getWindow() const;

        /// Gets window width.
        /**
         * @return Width of the window.
         */
        int getWidth() const;

        /// Gets window height.
        /**
         * @return Height of the window.
         */
        int getHeight() const;

    private:
        int width = 1024;
        int height = 768;
        GLFWwindow* window;
};
