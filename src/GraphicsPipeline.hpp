#pragma once

#include "Shader.hpp"
#include <vector>

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
        
        Shader vertexShader;
        Shader geometryShader;
        Shader fragmentShader;
        
        VkDevice device;
        VkPipelineLayout pipelineLayout;
        VkPipeline pipeline;
        
        std::vector<VkDescriptorSetLayout> descriptorSetLayouts;
};
