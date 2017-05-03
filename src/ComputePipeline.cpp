#include "ComputePipeline.hpp"

#include <iostream>
#include "update.comp.spv.hpp"

ComputePipeline::ComputePipeline(VkDevice device) : shader(UPDATE_COMP_SPV, UPDATE_COMP_SPV_LENGTH, device) {
    this->device = device;
    
    // Create shader stages.
    VkPipelineShaderStageCreateInfo shaderStageCreateInfo = createShaderStage(VK_SHADER_STAGE_COMPUTE_BIT, shader.getModule());
    
    VkPipelineShaderStageCreateInfo shaderStages[] = {shaderStageCreateInfo};
    
    // Pipeline layout.
    VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 0;
    pipelineLayoutInfo.pSetLayouts = nullptr;
    
    if (vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
        std::cerr << "Failed to create graphics pipeline layout." << std::endl;
        exit(-1);
    }
}

ComputePipeline::~ComputePipeline() {
    vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
}

VkPipelineShaderStageCreateInfo ComputePipeline::createShaderStage(VkShaderStageFlagBits flags, VkShaderModule module) {
    VkPipelineShaderStageCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    createInfo.stage = flags;
    createInfo.module = module;
    createInfo.pName = "main";
    
    return createInfo;
}
