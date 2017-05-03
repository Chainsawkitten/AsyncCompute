#include "StorageBuffer.hpp"

#include <iostream>
#include <cstring>

StorageBuffer::StorageBuffer(const void* data, unsigned int size, VkDevice device, VkPhysicalDevice physicalDevice, VkDescriptorPool descriptorPool, VkQueue graphicsQueue, VkCommandPool commandPool) {
    this->device = device;
    this->physicalDevice = physicalDevice;
    this->descriptorPool = descriptorPool;
    this->graphicsQueue = graphicsQueue;
    this->commandPool = commandPool;

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;

    // Staging buffer.
    createBuffer(size, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &stagingBuffer, &stagingBufferMemory);

    // Copy data to mapped memory.
    void* mappedMemory;
    vkMapMemory(device, stagingBufferMemory, 0, size, 0, &mappedMemory);
    memcpy(mappedMemory, data, size);
    vkUnmapMemory(device, stagingBufferMemory);

    // Actual buffer.
    createBuffer(size, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &buffer, &bufferMemory);
    
    // Copy from staging buffer to actual buffer.
    copyBuffer(stagingBuffer, buffer, size);

    // Create descriptor set.
    createDescriptorSetLayout();
    createDescriptorSet(size);
}

StorageBuffer::~StorageBuffer() {
    vkDestroyDescriptorSetLayout(device, descriptorSetLayout, nullptr);
    vkDestroyBuffer(device, buffer, nullptr);
    vkFreeMemory(device, bufferMemory, nullptr);
}

VkDescriptorSet StorageBuffer::getDescriptorSet() const {
    return descriptorSet;
}

void StorageBuffer::createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer* buffer, VkDeviceMemory* bufferMemory) {
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

void StorageBuffer::copyBuffer(VkBuffer source, VkBuffer destination, VkDeviceSize size){
    // Create command buffer allocation info.
    VkCommandBufferAllocateInfo allocationInfo = {};
    allocationInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocationInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocationInfo.commandPool = commandPool;
    allocationInfo.commandBufferCount = 1;

    // Create command buffer.
    VkCommandBuffer commandBuffer;
    vkAllocateCommandBuffers(device, &allocationInfo, &commandBuffer);

    // Command buffer begin info.
    VkCommandBufferBeginInfo beginInfo = {};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    // Start recording commands.
    vkBeginCommandBuffer(commandBuffer, &beginInfo);

    VkBufferCopy copyRegion = {};
    copyRegion.size = size;
    vkCmdCopyBuffer(commandBuffer, source, destination, 1, &copyRegion);

    vkEndCommandBuffer(commandBuffer);

    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(graphicsQueue);

    vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);
}

void StorageBuffer::createDescriptorSetLayout() {
    VkDescriptorSetLayoutBinding vertexLayoutBinding = {};
    vertexLayoutBinding.binding = 0;
    vertexLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
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

void StorageBuffer::createDescriptorSet(VkDeviceSize size) {
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
    descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    descriptorWrite.descriptorCount = 1;
    descriptorWrite.pBufferInfo = &bufferInfo;
    descriptorWrite.pImageInfo = nullptr;
    descriptorWrite.pTexelBufferView = nullptr;
    
    vkUpdateDescriptorSets(device, 1, &descriptorWrite, 0, nullptr);
}
