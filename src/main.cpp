#include "Renderer.hpp"
#include "Window.hpp"
#include <cstring>
#include "Particle.png.hpp"

int main(int argc, char* argv[]) {
    Window window;
    
    Renderer* renderer = new Renderer(window);
    renderer->setTexture(PARTICLE_PNG, PARTICLE_PNG_LENGTH);
    
    while(!glfwWindowShouldClose(window.getWindow())){
        glfwPollEvents();
        
        renderer->update();
        renderer->render();
        
        glfwSwapBuffers(window.getWindow());
    }
    
    delete renderer;
    
    return 0;
}
