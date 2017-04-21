#pragma once

#include <vulkan/vulkan.h>

/// Used to compile Vulkan shaders.
class VulkanShader {
    public:
        /// Create new Vulkan shader.
        /**
         * @param data SPIR-V data.
         * @param dataLength Length of the data.
         * @param device Logical Vulkan device.
         */
        VulkanShader(const char* data, unsigned int dataLength, VkDevice device);
        
        /// Destructor.
        ~VulkanShader();
        
    private:
        VkDevice device;
        VkShaderModule shaderModule;
};
