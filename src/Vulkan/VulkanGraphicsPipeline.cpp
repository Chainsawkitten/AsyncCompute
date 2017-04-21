#include "VulkanGraphicsPipeline.hpp"

#include "default.vert.spv.hpp"

VulkanGraphicsPipeline::VulkanGraphicsPipeline(VkDevice device) : vertexShader(DEFAULT_VERT_SPV, DEFAULT_VERT_SPV_LENGTH, device) {
    // Create shader stages.
    VkPipelineShaderStageCreateInfo vertexShaderStageCreateInfo = createShaderStage(VK_SHADER_STAGE_VERTEX_BIT, vertexShader.getModule());
    
    VkPipelineShaderStageCreateInfo shaderStages[] = {vertexShaderStageCreateInfo};
}

VulkanGraphicsPipeline::~VulkanGraphicsPipeline() {
    
}

VkPipelineShaderStageCreateInfo VulkanGraphicsPipeline::createShaderStage(VkShaderStageFlagBits flags, VkShaderModule module) {
    VkPipelineShaderStageCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    createInfo.stage = flags;
    createInfo.module = module;
    createInfo.pName = "main";
    
    return createInfo;
}
