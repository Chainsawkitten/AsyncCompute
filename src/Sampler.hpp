#pragma once

#include <vulkan/vulkan.h>

/// Used to sample a texture.
class Sampler {
    public:
        /// Create new sampler.
        /**
         * @param device Logical Vulkan device.
         */
        Sampler(VkDevice device);
        
        /// Destructor.
        ~Sampler();
        
        /// Get the sampler handle.
        /**
         * @return The sampler handle.
         */
        VkSampler getTextureSampler() const;
        
    private:
        VkDevice device;
        
        VkSampler textureSampler;
};
