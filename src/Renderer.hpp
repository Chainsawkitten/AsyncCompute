#pragma once

#include "Renderer.hpp"
#include <vulkan/vulkan.h>
#include <vector>
#include "Window.hpp"

class Texture;
class GraphicsPipeline;
class StorageBuffer;

/// Used to render the particles.
class Renderer {
    public:
        /// Create new renderer.
        /**
         * @param window The window to render in.
         */
        Renderer(Window& window);
        
        /// Destructor.
        ~Renderer();
        
        /// Set the texture to use for the particles.
        /**
         * @param textureData The texture data.
         * @param dataLength The length of the texture data.
         */
        void setTexture(const char* textureData, unsigned int dataLength);
        
        /// Render image to screen.
        void render();
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
        
        Texture* particleTexture;
        GraphicsPipeline* graphicsPipeline;
        StorageBuffer* particleBuffer;
};
