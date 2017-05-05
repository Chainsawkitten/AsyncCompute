#include "UniformBuffer.hpp"

#include <iostream>
#include <cstring>

UniformBuffer::UniformBuffer(const void* data, unsigned int size, VkDevice device, VkPhysicalDevice physicalDevice, VkDescriptorPool descriptorPool, VkShaderStageFlags flags) : Buffer(device, physicalDevice) {
    this->device = device;
    this->physicalDevice = physicalDevice;
    this->descriptorPool = descriptorPool;
    
    createBuffer(size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &buffer, &bufferMemory);
    
    // Copy data to mapped memory.
    setData(data, size);
    
    // Create descriptor set.
    createDescriptorSetLayout(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, flags);
    createDescriptorSet(size);
}

UniformBuffer::~UniformBuffer() {
    vkDestroyBuffer(device, buffer, nullptr);
    vkFreeMemory(device, bufferMemory, nullptr);
}

VkDescriptorSet UniformBuffer::getDescriptorSet() const {
    return descriptorSet;
}

void UniformBuffer::setData(const void* data, unsigned int size) {
    void* mappedMemory;
    vkMapMemory(device, bufferMemory, 0, size, 0, &mappedMemory);
    memcpy(mappedMemory, data, size);
    vkUnmapMemory(device, bufferMemory);
}

void UniformBuffer::createDescriptorSet(VkDeviceSize size) {
    // Allocate descriptor set.
    VkDescriptorSetAllocateInfo allocateInfo = {};
    allocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocateInfo.descriptorPool = descriptorPool;
    allocateInfo.descriptorSetCount = 1;
    allocateInfo.pSetLayouts = getDescriptorSetLayout();
    
    if (vkAllocateDescriptorSets(device, &allocateInfo, &descriptorSet) != VK_SUCCESS) {
        std::cerr << "Failed to allocate descriptor set" << std::endl;
        exit(-1);
    }
    
    // Update descriptor set.
    VkDescriptorBufferInfo bufferInfo = {};
    bufferInfo.buffer = buffer;
    bufferInfo.offset = 0;
    bufferInfo.range = size;
    
    VkWriteDescriptorSet descriptorWrite = {};
    descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrite.dstSet = descriptorSet;
    descriptorWrite.dstBinding = 0;
    descriptorWrite.dstArrayElement = 0;
    descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    descriptorWrite.descriptorCount = 1;
    descriptorWrite.pBufferInfo = &bufferInfo;
    descriptorWrite.pImageInfo = nullptr;
    descriptorWrite.pTexelBufferView = nullptr;
    
    vkUpdateDescriptorSets(device, 1, &descriptorWrite, 0, nullptr);
}
