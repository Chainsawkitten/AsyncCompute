#include "Renderer.hpp"
#include "../OpenGL/OpenGLRenderer.hpp"
#include "../Vulkan/VulkanRenderer.hpp"

Renderer::~Renderer() {
    
}

Renderer* Renderer::makeRenderer(BACKEND backend, Window & createdWindow){
    // Create and return backend.
    if(backend == BACKEND::OpenGL)
        return new OpenGLRenderer();
    
    else if(backend == BACKEND::Vulkan)
        return new VulkanRenderer();
    
    return nullptr;
}
