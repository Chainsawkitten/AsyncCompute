#include "Renderer.hpp"
#include "../OpenGL/OpenGLRenderer.hpp"
#include "../Vulkan/VulkanRenderer.hpp"

Renderer::~Renderer() {
    
}

Renderer* Renderer::makeRenderer(BACKEND backend, Window & window){
    // Create and return backend.
    if(backend == BACKEND::OpenGL)
        return new OpenGLRenderer(window);
    
    else if(backend == BACKEND::Vulkan)
        return new VulkanRenderer(window);
    
    return nullptr;
}

void Renderer::render() {

}
