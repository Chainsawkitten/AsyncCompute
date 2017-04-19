#include "Base/Renderer.hpp"
#include "Base/Window.hpp"
#include <cstring>

int main(int argc, char* argv[]) {
    Window window;
    
    // Choose backend based on command line arguments.
    Renderer::BACKEND backend = Renderer::BACKEND::OpenGL;
    if (argc >= 2 && strcmp(argv[1], "vulkan") == 0)
        backend = Renderer::BACKEND::Vulkan;
    
    Renderer* renderer = Renderer::makeRenderer(backend, window);
    
    while(!glfwWindowShouldClose(window.getWindow())){
        glfwPollEvents();

        renderer->render();

        glfwSwapBuffers(window.getWindow());
    }
    
    delete renderer;
    
    return 0;
}
