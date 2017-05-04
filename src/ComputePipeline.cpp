#include "ComputePipeline.hpp"

#include <iostream>
#include "update.comp.spv.hpp"

ComputePipeline::ComputePipeline(VkDevice device) : shader(UPDATE_COMP_SPV, UPDATE_COMP_SPV_LENGTH, device) {
    this->device = device;
    
    // Create shader stages.
    VkPipelineShaderStageCreateInfo shaderStageCreateInfo = createShaderStage(VK_SHADER_STAGE_COMPUTE_BIT, shader.getModule());
    
    // Descriptor set layouts.
    createDescriptorSetLayouts();
    
    // Pipeline layout.
    VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = descriptorSetLayouts.size();
    pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data();
    
    if (vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
        std::cerr << "Failed to create compute pipeline layout." << std::endl;
        exit(-1);
    }
    
    // Create pipeline.
    VkComputePipelineCreateInfo pipelineInfo = {};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
    pipelineInfo.layout = pipelineLayout;
    pipelineInfo.stage = shaderStageCreateInfo;
    
    if (vkCreateComputePipelines(device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &pipeline) != VK_SUCCESS) {
        std::cerr << "Failed to create compute pipeline." << std::endl;
        exit(-1);
    }
}

ComputePipeline::~ComputePipeline() {
    vkDestroyPipeline(device, pipeline, nullptr);
    vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
}

VkPipeline ComputePipeline::getPipeline() const {
    return pipeline;
}

VkPipelineShaderStageCreateInfo ComputePipeline::createShaderStage(VkShaderStageFlagBits flags, VkShaderModule module) {
    VkPipelineShaderStageCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    createInfo.stage = flags;
    createInfo.module = module;
    createInfo.pName = "main";
    
    return createInfo;
}

void ComputePipeline::createDescriptorSetLayouts() {
    VkDescriptorSetLayout layout;
    
    // Position storage buffer.
    VkDescriptorSetLayoutBinding layoutBinding = {};
    layoutBinding.binding = 0;
    layoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    layoutBinding.descriptorCount = 1;
    layoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_COMPUTE_BIT;
    
    VkDescriptorSetLayoutCreateInfo layoutInfo = {};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = 1;
    layoutInfo.pBindings = &layoutBinding;
    
    if (vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &layout) != VK_SUCCESS) {
        std::cerr << "Failed to create descriptor set layout." << std::endl;
        exit(-1);
    }
    
    descriptorSetLayouts.push_back(layout);
}
