#pragma once

#include "Buffer.hpp"
#include <vulkan/vulkan.h>

/// Used for storing particle data.
class StorageBuffer : public Buffer {
    public:
        /// Create new storage buffer.
        /**
         * @param data Buffer data.
         * @param size Buffer size.
         * @param device Logical Vulkan device.
         * @param physicalDevice Physical Vulkan device.
         * @param descriptorPool Pool to allocate descriptor sets from.
         */
        StorageBuffer(const void* data, unsigned int size, VkDevice device, VkPhysicalDevice physicalDevice, VkDescriptorPool descriptorPool, VkQueue graphicsQueue, VkCommandPool commandPool);
        
        /// Destructor.
        ~StorageBuffer();
        
        /// Get descriptor set.
        /**
         * @return The descriptor set.
         */
        VkDescriptorSet getDescriptorSet() const;
        
    private:
        void copyBuffer(VkBuffer source, VkBuffer destination, VkDeviceSize size);
        void createDescriptorSet(VkDeviceSize size);
        
        VkDevice device;
        VkQueue graphicsQueue;
        VkCommandPool commandPool;
        VkPhysicalDevice physicalDevice;
        VkDescriptorPool descriptorPool;
        
        VkBuffer buffer;
        VkDeviceMemory bufferMemory;
        VkDescriptorSet descriptorSet;
};
