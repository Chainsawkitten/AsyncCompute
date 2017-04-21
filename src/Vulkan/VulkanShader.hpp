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
        
        /// Get the shader module.
        /**
         * @return The shader module.
         */
        VkShaderModule getModule() const;
        
    private:
        VkDevice device;
        VkShaderModule shaderModule;
};
