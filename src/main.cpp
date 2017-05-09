#include "Renderer.hpp"
#include "Window.hpp"
#include <cstring>
#include <iostream>
#include "Particle.png.hpp"

int main(int argc, char* argv[]) {
    Window window;
    
    bool measure = false;
    if (argc > 1 && strcmp(argv[1], "measure") == 0) {
        std::cout << "Measuring..." << std::endl;
        measure = true;
    }
    
    Renderer* renderer = new Renderer(window);
    renderer->setTexture(PARTICLE_PNG, PARTICLE_PNG_LENGTH);
    
    double lastTime = glfwGetTime();
    while(!glfwWindowShouldClose(window.getWindow())){
        glfwPollEvents();
        
        double deltaTime = glfwGetTime() - lastTime;
        lastTime = glfwGetTime();
        
        renderer->update(deltaTime);
        renderer->render();
        
        glfwSwapBuffers(window.getWindow());
    }
    
    delete renderer;
    
    return 0;
}
