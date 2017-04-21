#pragma once

#include "VulkanShader.hpp"

/// Pipeline used to render particles.
class VulkanGraphicsPipeline {
    public:
        /// Create new graphics pipeline.
        /**
         * @param device Logical Vulkan device.
         * @param swapChainExtent Size of the swap chain buffers.
         */
        VulkanGraphicsPipeline(VkDevice device, VkExtent2D swapChainExtent);
        
        /// Destructor.
        ~VulkanGraphicsPipeline();
        
    private:
        VkPipelineShaderStageCreateInfo createShaderStage(VkShaderStageFlagBits flags, VkShaderModule module);
        
        VulkanShader vertexShader;
        
        VkDevice device;
        VkPipelineLayout pipelineLayout;
};
