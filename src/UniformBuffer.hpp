#pragma once

#include <vulkan/vulkan.h>

/// Used for storing camera data.
class UniformBuffer {
    public:
        /// Create new uniform buffer.
        /**
         * @param data Buffer data.
         * @param size Buffer size.
         * @param device Logical Vulkan device.
         * @param physicalDevice Physical Vulkan device.
         * @param descriptorPool Pool to allocate descriptor sets from.
         * @param flags Flags describing which shader stages the buffer will be used in.
         */
        UniformBuffer(const void* data, unsigned int size, VkDevice device, VkPhysicalDevice physicalDevice, VkDescriptorPool descriptorPool, VkShaderStageFlags flags);
        
        /// Destructor.
        ~UniformBuffer();
        
        /// Get the descriptor set.
        /**
         * @return The descriptor set.
         */
        VkDescriptorSet getDescriptorSet() const;
        
    private:
        void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer* buffer, VkDeviceMemory* bufferMemory);
        void createDescriptorSetLayout(VkShaderStageFlags flags);
        void createDescriptorSet(VkDeviceSize size);
        
        VkDevice device;
        VkPhysicalDevice physicalDevice;
        VkDescriptorPool descriptorPool;
        
        VkBuffer buffer;
        VkDeviceMemory bufferMemory;
        VkDescriptorSetLayout descriptorSetLayout;
        VkDescriptorSet descriptorSet;
};
