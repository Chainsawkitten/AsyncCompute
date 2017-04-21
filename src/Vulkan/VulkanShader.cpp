#include "VulkanShader.hpp"

#include <iostream>

VulkanShader::VulkanShader(const char* data, unsigned int dataLength, VkDevice device) {
    this->device = device;
    
    VkShaderModuleCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = dataLength;
    createInfo.pCode = (uint32_t*) data;
    
    if (vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
        std::cerr << "Failed to create shader module." << std::endl;
        exit(-1);
    }
}

VulkanShader::~VulkanShader() {
    vkDestroyShaderModule(device, shaderModule, nullptr);
}

VkShaderModule VulkanShader::getModule() const {
    return shaderModule;
}
