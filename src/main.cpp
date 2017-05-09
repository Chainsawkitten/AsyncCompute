#include "Renderer.hpp"
#include "Window.hpp"
#include <cstring>
#include "Particle.png.hpp"

int main(int argc, char* argv[]) {
    Window window;
    
    Renderer* renderer = new Renderer(window);
    renderer->setTexture(PARTICLE_PNG, PARTICLE_PNG_LENGTH);
    renderer->recordCommandBuffers();
    
    double lastTime = glfwGetTime();
    while(!glfwWindowShouldClose(window.getWindow())){
        glfwPollEvents();
        
        double deltaTime = glfwGetTime() - lastTime;
        glfwSetWindowTitle(window.getWindow(), (std::to_string(deltaTime * 1000.0) + " ms").c_str());
        lastTime = glfwGetTime();
        
        renderer->update(deltaTime);
        renderer->render();
        
        glfwSwapBuffers(window.getWindow());
    }
    
    delete renderer;
    
    return 0;
}
