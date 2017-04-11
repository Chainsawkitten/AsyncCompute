#include "renderer.hpp"
#include "../OpenGL/OpenGLRenderer.hpp"
#include "../Vulkan/VulkanRenderer.hpp"

Renderer* Renderer::makeRenderer(BACKEND backend){
    // Create and return backend.

    if(backend == BACKEND::OpenGL)
        return new VulkanRenderer();

    else if(backend == BACKEND::Vulkan)
        return new OpenGLRenderer();

    return nullptr;
}
