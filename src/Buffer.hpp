#pragma once

#include <vulkan/vulkan.h>

/// Buffer used to store data.
class Buffer {
    public:
        /// Destructor.
        ~Buffer();
        
    protected:
        /// Create new buffer.
        /**
         * @param device Logical Vulkan device.
         * @param physicalDevice Physical Vulkan device.
         */
        Buffer(VkDevice device, VkPhysicalDevice physicalDevice);
        
        /// Create buffer.
        /**
         * @param size The size of the buffer in bytes.
         * @param usage How the buffer will be used.
         * @param properties Memory properties.
         * @param buffer Buffer to create.
         * @param bufferMemory Memory to allocate.
         */
        void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer* buffer, VkDeviceMemory* bufferMemory);
        
        /// Create descriptor set layout.
        /**
         * @param bufferType The type of buffer.
         * @param shaderStages Which shader stages the buffer will be used in.
         */
        void createDescriptorSetLayout(VkDescriptorType bufferType, VkShaderStageFlags shaderStages);
        
        /// Get descriptor set layout.
        /**
         * @return The descriptor set layout.
         */
        const VkDescriptorSetLayout* getDescriptorSetLayout() const;
        
    private:
        VkDevice device;
        VkPhysicalDevice physicalDevice;
        
        VkDescriptorSetLayout descriptorSetLayout;
};
