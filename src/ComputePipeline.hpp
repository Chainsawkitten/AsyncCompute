#pragma once

#include "Shader.hpp"
#include <vector>

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
        
        /// Get pipeline layout.
        /**
         * @return The pipeline layout.
         */
        VkPipelineLayout getPipelineLayout() const;
        
    private:
        VkPipelineShaderStageCreateInfo createShaderStage(VkShaderStageFlagBits flags, VkShaderModule module);
        void createDescriptorSetLayouts();
        
        Shader shader;
        
        VkDevice device;
        VkPipelineLayout pipelineLayout;
        VkPipeline pipeline;
        
        std::vector<VkDescriptorSetLayout> descriptorSetLayouts;
};
