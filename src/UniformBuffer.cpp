#include "UniformBuffer.hpp"

#include <iostream>
#include <cstring>

UniformBuffer::UniformBuffer(const void* data, unsigned int size, VkDevice device, VkPhysicalDevice physicalDevice, VkDescriptorPool descriptorPool, VkShaderStageFlags flags) : Buffer(device, physicalDevice, descriptorPool) {
    this->device = device;
    this->physicalDevice = physicalDevice;
    this->descriptorPool = descriptorPool;
    
    createBuffer(size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &buffer, &bufferMemory);
    
    // Copy data to mapped memory.
    setData(data, size);
    
    // Create descriptor set.
    createDescriptorSetLayout(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, flags);
    createDescriptorSet(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, buffer, size);
}

UniformBuffer::~UniformBuffer() {
    vkDestroyBuffer(device, buffer, nullptr);
    vkFreeMemory(device, bufferMemory, nullptr);
}

void UniformBuffer::setData(const void* data, unsigned int size) {
    void* mappedMemory;
    vkMapMemory(device, bufferMemory, 0, size, 0, &mappedMemory);
    memcpy(mappedMemory, data, size);
    vkUnmapMemory(device, bufferMemory);
}
