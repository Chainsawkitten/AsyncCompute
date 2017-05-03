#include "ComputePipeline.hpp"

#include <iostream>
#include "update.comp.spv.hpp"

ComputePipeline::ComputePipeline(VkDevice device) : shader(UPDATE_COMP_SPV, UPDATE_COMP_SPV_LENGTH, device) {
    this->device = device;
    
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
