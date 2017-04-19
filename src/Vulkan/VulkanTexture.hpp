#pragma once

/// A texture used for particles.
class VulkanTexture {
    public:
        /// Load a texture from memory.
        /**
         * @param data Texture data.
         * @param length Length of the data.
         */
        VulkanTexture(const char* data, unsigned int length);
        
        /// Destructor.
        ~VulkanTexture();
};
