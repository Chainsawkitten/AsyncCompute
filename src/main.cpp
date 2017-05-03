#include "Base/Renderer.hpp"
#include "Base/Window.hpp"
#include <cstring>
#include "Particle.png.hpp"

int main(int argc, char* argv[]) {
    Window window;
    
    Renderer* renderer = Renderer::makeRenderer(Renderer::BACKEND::Vulkan, window);
    renderer->setTexture(PARTICLE_PNG, PARTICLE_PNG_LENGTH);
    
    while(!glfwWindowShouldClose(window.getWindow())){
        glfwPollEvents();
        
        renderer->render();
        
        glfwSwapBuffers(window.getWindow());
    }
    
    delete renderer;
    
    return 0;
}
