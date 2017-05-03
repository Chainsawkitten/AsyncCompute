#include "ComputePipeline.hpp"

#include <iostream>
#include "update.comp.spv.hpp"

ComputePipeline::ComputePipeline(VkDevice device) : shader(UPDATE_COMP_SPV, UPDATE_COMP_SPV_LENGTH, device) {
    this->device = device;
    
    // Create shader stages.
    VkPipelineShaderStageCreateInfo shaderStageCreateInfo = createShaderStage(VK_SHADER_STAGE_COMPUTE_BIT, shader.getModule());
    
    // Pipeline layout.
    VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 0;
    pipelineLayoutInfo.pSetLayouts = nullptr;
    
    if (vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
        std::cerr << "Failed to create graphics pipeline layout." << std::endl;
        exit(-1);
    }
    
    // Create pipeline.
    VkComputePipelineCreateInfo pipelineInfo = {};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
    pipelineInfo.layout = pipelineLayout;
    pipelineInfo.stage = shaderStageCreateInfo;
    
    if (vkCreateComputePipelines(device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &pipeline));
}

ComputePipeline::~ComputePipeline() {
    vkDestroyPipeline(device, pipeline, nullptr);
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
