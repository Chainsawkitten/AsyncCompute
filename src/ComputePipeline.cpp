#include "ComputePipeline.hpp"

#include "update.comp.spv.hpp"

ComputePipeline::ComputePipeline(VkDevice device) : shader(UPDATE_COMP_SPV, UPDATE_COMP_SPV_LENGTH, device) {
    
}

ComputePipeline::~ComputePipeline() {
    
}
