#include "VulkanGraphicsPipeline.hpp"

#include "default.vert.spv.hpp"

VulkanGraphicsPipeline::VulkanGraphicsPipeline(VkDevice device) : vertexShader(DEFAULT_VERT_SPV, DEFAULT_VERT_SPV_LENGTH, device) {
    
}

VulkanGraphicsPipeline::~VulkanGraphicsPipeline() {
    
}
