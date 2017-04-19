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
        
    private:
        VkDevice device;
        
        VkSampler textureSampler;
};
