#pragma once

#include "Shader.hpp"

/// Pipeline used to render particles.
class GraphicsPipeline {
    public:
        /// Create new graphics pipeline.
        /**
         * @param device Logical Vulkan device.
         * @param swapChainExtent Size of the swap chain buffers.
         * @param renderPass Render pass.
         */
        GraphicsPipeline(VkDevice device, VkExtent2D swapChainExtent, VkRenderPass renderPass);
        
        /// Destructor.
        ~GraphicsPipeline();
        
    private:
        VkPipelineShaderStageCreateInfo createShaderStage(VkShaderStageFlagBits flags, VkShaderModule module);
        
        Shader vertexShader;
        
        VkDevice device;
        VkPipelineLayout pipelineLayout;
        VkPipeline pipeline;
};
