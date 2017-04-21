#include "VulkanStorageBuffer.hpp"

#include <iostream>
#include <cstring>

VulkanStorageBuffer::VulkanStorageBuffer(const void* data, unsigned int size, VkDevice device, VkPhysicalDevice physicalDevice) {
    this->device = device;
    this->physicalDevice = physicalDevice;
    
    createBuffer(size, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &buffer, &bufferMemory);
    
    // Copy data to mapped memory.
    void* mappedMemory;
    vkMapMemory(device, bufferMemory, 0, size, 0, &mappedMemory);
    memcpy(mappedMemory, data, size);
    vkUnmapMemory(device, bufferMemory);
    
    // Create descriptor set layout.
    createDescriptorSetLayout();
}

VulkanStorageBuffer::~VulkanStorageBuffer() {
    vkDestroyDescriptorSetLayout(device, descriptorSetLayout, nullptr);
    vkDestroyBuffer(device, buffer, nullptr);
    vkFreeMemory(device, bufferMemory, nullptr);
}

void VulkanStorageBuffer::createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer* buffer, VkDeviceMemory* bufferMemory) {
    // Create buffer.
    VkBufferCreateInfo bufferInfo = {};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    
    if (vkCreateBuffer(device, &bufferInfo, nullptr, buffer) != VK_SUCCESS) {
        std::cerr << "Failed to create buffer." << std::endl;
        exit(-1);
    }
    
    // Get information about device memory.
    VkMemoryRequirements memoryRequirements;
    vkGetBufferMemoryRequirements(device, *buffer, &memoryRequirements);
    
    VkPhysicalDeviceMemoryProperties memoryProperties;
    vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memoryProperties);
    
    // Find suitable memory type.
    uint32_t memoryType;
    for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++) {
        if (memoryRequirements.memoryTypeBits & (1 << i) && (memoryProperties.memoryTypes[i].propertyFlags & properties) == properties )
            memoryType = i;
    }
    
    // Allocate buffer memory.
    VkMemoryAllocateInfo allocateInfo = {};
    allocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocateInfo.allocationSize = memoryRequirements.size;
    allocateInfo.memoryTypeIndex = memoryType;
    
    if (vkAllocateMemory(device, &allocateInfo, nullptr, bufferMemory) != VK_SUCCESS) {
        std::cerr << "Failed to allocate buffer memory." << std::endl;
        exit(-1);
    }
    
    vkBindBufferMemory(device, *buffer, *bufferMemory, 0);
}

void VulkanStorageBuffer::createDescriptorSetLayout() {
    VkDescriptorSetLayoutBinding vertexLayoutBinding = {};
    vertexLayoutBinding.binding = 0;
    vertexLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC;
    vertexLayoutBinding.descriptorCount = 1;
    vertexLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
    vertexLayoutBinding.pImmutableSamplers = nullptr;
    
    VkDescriptorSetLayoutCreateInfo layoutInfo = {};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = 1;
    layoutInfo.pBindings = &vertexLayoutBinding;
    
    if (vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &descriptorSetLayout)) {
        std::cerr << "Could not create descriptor set layout!" << std::endl;
        exit(-1);
    }
}
