#include "Base/Renderer.hpp"
#include "Base/Window.hpp"

int main() {
    Window window;

    Renderer* renderer = Renderer::makeRenderer(Renderer::BACKEND::OpenGL, window);

    while(!glfwWindowShouldClose(window.getWindow())){
        glfwPollEvents();
        glfwSwapBuffers(window.getWindow());
    }

    delete renderer;
    
    return 0;
}
