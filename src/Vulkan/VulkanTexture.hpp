#pragma once

#include <vulkan/vulkan.h>

/// A texture used for particles.
class VulkanTexture {
    public:
        /// Load a texture from memory.
        /**
         * @param data Texture data.
         * @param length Length of the data.
         * @param device Logical Vulkan device.
         * @param physicalDevice Physical Vulkan device.
         */
        VulkanTexture(const char* data, unsigned int length, VkDevice device, VkPhysicalDevice physicalDevice);
        
        /// Destructor.
        ~VulkanTexture();
        
    private:
        void createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage* image, VkDeviceMemory* imageMemory);
        uint32_t findMemoryType(uint32_t memoryTypeBits, VkMemoryPropertyFlags properties);
        
        VkDevice device;
        VkPhysicalDevice physicalDevice;
        
        VkImage stagingImage;
        VkDeviceMemory stagingImageMemory;
        
        VkImage textureImage;
        VkDeviceMemory textureImageMemory;
};
