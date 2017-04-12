#pragma once

#include "../Base/Renderer.hpp"
#include <vulkan/vulkan.h>
#include "../Base/Window.hpp"

/// Vulkan implementation of the renderer.
class VulkanRenderer : public Renderer {
    public:
        /// Create new Vulkan renderer.
        /**
         * @param window The window to render in.
         */
        VulkanRenderer(Window& window);
        
        /// Destructor.
        ~VulkanRenderer() final;
        
    private:
        void createInstance();
        void setupDebugCallback();
        void createDevice();
        void createSurface(GLFWwindow* window);
        
        VkInstance instance;
#ifndef NDEBUG
        VkDebugReportCallbackEXT callback;
#endif
        
        VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
        VkDevice device;
        
        VkSurfaceKHR surface;
};
