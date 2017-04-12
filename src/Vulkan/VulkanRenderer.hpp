#pragma once

#include "../Base/Renderer.hpp"

/// Vulkan implementation of the renderer.
class VulkanRenderer : public Renderer {
    public:
        /// Create new Vulkan renderer.
        VulkanRenderer();
        
        /// Destructor.
        ~VulkanRenderer() final;
};
