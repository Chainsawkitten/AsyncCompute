#include "Shader.hpp"

#include <iostream>

Shader::Shader(const char* data, unsigned int dataLength, VkDevice device) {
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

Shader::~Shader() {
    vkDestroyShaderModule(device, shaderModule, nullptr);
}

VkShaderModule Shader::getModule() const {
    return shaderModule;
}
