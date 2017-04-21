#pragma once

/// Used to compile Vulkan shaders.
class VulkanShader {
    public:
        /// Create new Vulkan shader.
        /**
         * @param data SPIR-V data.
         * @param dataLength Length of the data.
         */
        VulkanShader(const char* data, unsigned int dataLength);
        
        /// Destructor.
        ~VulkanShader();
};
