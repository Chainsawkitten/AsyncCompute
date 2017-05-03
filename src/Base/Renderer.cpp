#include "Renderer.hpp"
#include "../Vulkan/VulkanRenderer.hpp"

Renderer::~Renderer() {
    
}

Renderer* Renderer::makeRenderer(BACKEND backend, Window & window){
    return new VulkanRenderer(window);
}
