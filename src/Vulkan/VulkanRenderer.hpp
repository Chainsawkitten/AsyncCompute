#pragma once

#include "../Base/Renderer.hpp"
#include <vulkan/vulkan.h>
#include <vector>
#include "../Base/Window.hpp"

class VulkanTexture;
class VulkanGraphicsPipeline;

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
        
        /// Set the texture to use for the particles.
        /**
         * @param textureData The texture data.
         * @param dataLength The length of the texture data.
         */
        void setTexture(const char* textureData, unsigned int dataLength) final;
        
        /// Render image to screen.
        void render() final;
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
        
        void createRenderPass(VkFormat format);
        void createFramebuffers();
        void createCommandPools();
        void createCommandBuffers();
        void createDescriptorPool();
        
        void createSemaphores();
        void createFence();
        
        VkInstance instance;
#ifndef NDEBUG
        VkDebugReportCallbackEXT callback;
#endif
        
        VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
        VkDevice device;
        int graphicsFamily;
        int computeFamily;
        VkQueue graphicsQueue;
        VkQueue computeQueue;
        VkQueue presentQueue;
        
        VkSurfaceKHR surface;
        VkSwapchainKHR swapChain;
        VkExtent2D swapChainExtent;
        std::vector<VkImage> swapChainImages;
        std::vector<VkImageView> swapChainImageViews;
        
        VkRenderPass renderPass;
        std::vector<VkFramebuffer> swapChainFramebuffers;
        VkCommandPool graphicsCommandPool;
        VkCommandPool computeCommandPool;
        VkCommandBuffer graphicsCommandBuffer;
        VkCommandBuffer computeCommandBuffer;
        VkDescriptorPool descriptorPool;
        
        VkSemaphore imageAvailableSemaphore;
        VkSemaphore renderFinishedSemaphore;
        uint32_t imageIndex = 0;
        VkFence fence;
        
        VulkanTexture* particleTexture;
        VulkanGraphicsPipeline* graphicsPipeline;
};
