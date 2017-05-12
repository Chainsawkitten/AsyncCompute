#pragma once

#include "Renderer.hpp"
#include <vulkan/vulkan.h>
#include <vector>
#include "Window.hpp"
#include "Camera.hpp"

class Texture;
class GraphicsPipeline;
class ComputePipeline;
class StorageBuffer;
class UniformBuffer;

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
        
        /// Record command buffers.
        void recordCommandBuffers();
        
        /// Update boids and render frame.
        /**
         * @param deltaTime Time since last frame (in seconds).
         */
        void frame(float deltaTime);
        
    private:
        struct SwapChainSupport {
            VkSurfaceCapabilitiesKHR capabilities;
            std::vector<VkSurfaceFormatKHR> formats;
            std::vector<VkPresentModeKHR> presentModes;
        };
        struct Particle {
            glm::vec4 position;
            glm::vec4 velocity;
        };
        struct CameraUniform {
            glm::mat4 viewProjectionMatrix;
            glm::vec4 cameraPosition;
            glm::vec4 cameraUp;
        };
        struct UpdateUniform {
            float deltaTime;
            unsigned int particleCount;
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
        void createFences();
        
        void recordUpdateCommandBuffer(int frame);
        void recordRenderCommandBuffer(int frame);
        
        void update(float deltaTime);
        void render();
        
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
        VkCommandBuffer graphicsCommandBuffers[2];
        VkCommandBuffer computeCommandBuffers[2];
        VkDescriptorPool descriptorPool;
        
        VkSemaphore imageAvailableSemaphore;
        VkSemaphore renderFinishedSemaphore;
        uint32_t imageIndex = 0;
        VkFence graphicsFence;
        VkFence computeFence;
        
        Texture* particleTexture;
        GraphicsPipeline* graphicsPipeline;
        ComputePipeline* computePipeline;
        int particleCount = 20000;
        StorageBuffer* particleBuffer[2];
        int bufferIndex = 0;
        UniformBuffer* cameraBuffer;
        UniformBuffer* updateBuffer;
        
        Camera camera;
};
