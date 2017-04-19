#pragma once

#include <vulkan/vulkan.h>
#include "VulkanSampler.hpp"

/// A texture used for particles.
class VulkanTexture {
    public:
        /// Load a texture from memory.
        /**
         * @param data Texture data.
         * @param length Length of the data.
         * @param device Logical Vulkan device.
         * @param physicalDevice Physical Vulkan device.
         * @param commandPool Pool to allocate commands from.
         * @param graphicsQueue Graphics queue.
         */
        VulkanTexture(const char* data, unsigned int length, VkDevice device, VkPhysicalDevice physicalDevice, VkCommandPool commandPool, VkQueue graphicsQueue);
        
        /// Destructor.
        ~VulkanTexture();
        
    private:
        void createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage* image, VkDeviceMemory* imageMemory);
        uint32_t findMemoryType(uint32_t memoryTypeBits, VkMemoryPropertyFlags properties);
        void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
        void copyImage(VkImage srcImage, VkImage dstImage, uint32_t width, uint32_t height);
        VkCommandBuffer beginSingleTimeCommands();
        void endSingleTimeCommands(VkCommandBuffer commandBuffer);
        void createImageView(VkImage image, VkFormat format, VkImageView* imageView);
        
        VkDevice device;
        VkPhysicalDevice physicalDevice;
        VkCommandPool commandPool;
        VkQueue graphicsQueue;
        
        VkImage stagingImage;
        VkDeviceMemory stagingImageMemory;
        
        VkImage textureImage;
        VkDeviceMemory textureImageMemory;
        
        VkImageView textureImageView;
        
        VulkanSampler sampler;
};
