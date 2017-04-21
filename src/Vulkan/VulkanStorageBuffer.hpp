#pragma once

#include <vulkan/vulkan.h>

/// Used for storing particle data.
class VulkanStorageBuffer {
    public:
        /// Create new storage buffer.
        /**
         * @param data Buffer data.
         * @param size Buffer size.
         * @param device Logical Vulkan device.
         * @param physicalDevice Physical Vulkan device.
         */
        VulkanStorageBuffer(const void* data, unsigned int size, VkDevice device, VkPhysicalDevice physicalDevice);
        
        /// Destructor.
        ~VulkanStorageBuffer();
        
    private:
        void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer* buffer, VkDeviceMemory* bufferMemory);
        void createDescriptorSetLayout();
        
        VkDevice device;
        VkPhysicalDevice physicalDevice;
        
        VkBuffer buffer;
        VkDeviceMemory bufferMemory;
        VkDescriptorSetLayout descriptorSetLayout;
};
