#pragma once

#include <vulkan/vulkan.h>

/// Used to compile SPIR-V shaders.
class Shader {
    public:
        /// Create new shader.
        /**
         * @param data SPIR-V data.
         * @param dataLength Length of the data.
         * @param device Logical Vulkan device.
         */
        Shader(const char* data, unsigned int dataLength, VkDevice device);
        
        /// Destructor.
        ~Shader();
        
        /// Get the shader module.
        /**
         * @return The shader module.
         */
        VkShaderModule getModule() const;
        
    private:
        VkDevice device;
        VkShaderModule shaderModule;
};
