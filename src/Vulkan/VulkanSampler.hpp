#pragma once

#include <vulkan/vulkan.h>

/// Used to sample a texture.
class VulkanSampler {
    public:
        /// Create new sampler.
        /**
         * @param device Logical Vulkan device.
         */
        VulkanSampler(VkDevice device);
        
        /// Destructor.
        ~VulkanSampler();
        
        /// Get the sampler handle.
        /**
         * @return The sampler handle.
         */
        VkSampler getTextureSampler() const;
        
    private:
        VkDevice device;
        
        VkSampler textureSampler;
};
