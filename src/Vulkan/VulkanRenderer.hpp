#pragma once

#include "../Base/Renderer.hpp"
#include <vulkan/vulkan.h>
#include <vector>
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
        struct SwapChainSupport {
            VkSurfaceCapabilitiesKHR capabilities;
            std::vector<VkSurfaceFormatKHR> formats;
            std::vector<VkPresentModeKHR> presentModes;
        };
        
        void createInstance();
        void setupDebugCallback();
        void createDevice();
        void createSurface(GLFWwindow* window);
        
        VkFormat createSwapChain(unsigned int width, unsigned int height);
        SwapChainSupport querySwapChainSupport();
        VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
        VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
        VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, unsigned int width, unsigned int height);
        void createImageViews(VkFormat format);
        
        VkInstance instance;
#ifndef NDEBUG
        VkDebugReportCallbackEXT callback;
#endif
        
        VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
        VkDevice device;
        VkQueue graphicsQueue;
        VkQueue computeQueue;
        VkQueue presentQueue;
        
        VkSurfaceKHR surface;
        VkSwapchainKHR swapChain;
        VkExtent2D swapChainExtent;
        std::vector<VkImage> swapChainImages;
        std::vector<VkImageView> swapChainImageViews;
};
