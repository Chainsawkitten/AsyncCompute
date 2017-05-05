#include "Buffer.hpp"

#include <iostream>

Buffer::~Buffer() {
    vkDestroyDescriptorSetLayout(device, descriptorSetLayout, nullptr);
}

VkDescriptorSet Buffer::getDescriptorSet() const {
    return descriptorSet;
}

Buffer::Buffer(VkDevice device, VkPhysicalDevice physicalDevice, VkDescriptorPool descriptorPool) {
    this->device = device;
    this->physicalDevice = physicalDevice;
    this->descriptorPool = descriptorPool;
}

void Buffer::createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer* buffer, VkDeviceMemory* bufferMemory) {
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

void Buffer::createDescriptorSetLayout(VkDescriptorType bufferType, VkShaderStageFlags flags) {
    VkDescriptorSetLayoutBinding vertexLayoutBinding = {};
    vertexLayoutBinding.binding = 0;
    vertexLayoutBinding.descriptorType = bufferType;
    vertexLayoutBinding.descriptorCount = 1;
    vertexLayoutBinding.stageFlags = flags;
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

const VkDescriptorSetLayout* Buffer::getDescriptorSetLayout() const {
    return &descriptorSetLayout;
}

void Buffer::createDescriptorSet(VkDescriptorType bufferType, VkBuffer buffer, VkDeviceSize size) {
    // Allocate descriptor set.
    VkDescriptorSetAllocateInfo allocateInfo = {};
    allocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocateInfo.descriptorPool = descriptorPool;
    allocateInfo.descriptorSetCount = 1;
    allocateInfo.pSetLayouts = &descriptorSetLayout;
    
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
    descriptorWrite.descriptorType = bufferType;
    descriptorWrite.descriptorCount = 1;
    descriptorWrite.pBufferInfo = &bufferInfo;
    descriptorWrite.pImageInfo = nullptr;
    descriptorWrite.pTexelBufferView = nullptr;
    
    vkUpdateDescriptorSets(device, 1, &descriptorWrite, 0, nullptr);
}
