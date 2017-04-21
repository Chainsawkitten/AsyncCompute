#include "VulkanGraphicsPipeline.hpp"

#include "default.vert.spv.hpp"

VulkanGraphicsPipeline::VulkanGraphicsPipeline(VkDevice device, VkExtent2D swapChainExtent) : vertexShader(DEFAULT_VERT_SPV, DEFAULT_VERT_SPV_LENGTH, device) {
    // Create shader stages.
    VkPipelineShaderStageCreateInfo vertexShaderStageCreateInfo = createShaderStage(VK_SHADER_STAGE_VERTEX_BIT, vertexShader.getModule());
    
    VkPipelineShaderStageCreateInfo shaderStages[] = {vertexShaderStageCreateInfo};
    
    // Vertex input.
    VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.vertexBindingDescriptionCount = 0;
    vertexInputInfo.pVertexBindingDescriptions = nullptr;
    vertexInputInfo.vertexAttributeDescriptionCount = 0;
    vertexInputInfo.pVertexAttributeDescriptions = nullptr;
    
    // Input assembly.
    VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssembly.primitiveRestartEnable = VK_FALSE;
    
    // Viewport.
    VkViewport viewport = {};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = swapChainExtent.width;
    viewport.height = swapChainExtent.height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    
    // Scissor.
    VkRect2D scissor = {};
    scissor.offset = {0, 0};
    scissor.extent = swapChainExtent;
    
    // Viewport state info.
    VkPipelineViewportStateCreateInfo viewportState = {};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.pViewports = &viewport;
    viewportState.scissorCount = 1;
    viewportState.pScissors = &scissor;
}

VulkanGraphicsPipeline::~VulkanGraphicsPipeline() {
    
}

VkPipelineShaderStageCreateInfo VulkanGraphicsPipeline::createShaderStage(VkShaderStageFlagBits flags, VkShaderModule module) {
    VkPipelineShaderStageCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    createInfo.stage = flags;
    createInfo.module = module;
    createInfo.pName = "main";
    
    return createInfo;
}
