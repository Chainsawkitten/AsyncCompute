#pragma once

#include "Shader.hpp"

/// Pipeline used to update particles.
class ComputePipeline {
    public:
        /// Create new compute pipeline.
        /**
         * @param device Logical Vulkan device.
         */
        ComputePipeline(VkDevice device);
        
        /// Destructor.
        ~ComputePipeline();
        
        /// Get pipeline.
        /**
         * @return The pipeline.
         */
        VkPipeline getPipeline() const;
        
    private:
        VkPipelineShaderStageCreateInfo createShaderStage(VkShaderStageFlagBits flags, VkShaderModule module);
        
        Shader shader;
        
        VkDevice device;
        VkPipelineLayout pipelineLayout;
        VkPipeline pipeline;
};
