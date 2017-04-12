#pragma once

#include "../Base/Renderer.hpp"
#include <vulkan/vulkan.h>

/// Vulkan implementation of the renderer.
class VulkanRenderer : public Renderer {
    public:
        /// Create new Vulkan renderer.
        VulkanRenderer();
        
        /// Destructor.
        ~VulkanRenderer() final;
        
    private:
        void createInstance();
        void setupDebugCallback();
        
        VkInstance instance;
#ifndef NDEBUG
        VkDebugReportCallbackEXT callback;
#endif
};
