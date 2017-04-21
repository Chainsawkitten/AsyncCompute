#pragma once

#include "VulkanShader.hpp"

/// Pipeline used to render particles.
class VulkanGraphicsPipeline {
    public:
        /// Create new graphics pipeline.
        /**
         * @param device Logical Vulkan device.
         */
        VulkanGraphicsPipeline(VkDevice device);
        
        /// Destructor.
        ~VulkanGraphicsPipeline();
        
    private:
        VkPipelineShaderStageCreateInfo createShaderStage(VkShaderStageFlagBits flags, VkShaderModule module);
        
        VulkanShader vertexShader;
};
