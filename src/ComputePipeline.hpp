#pragma once

#include "Shader.hpp"

/// Pipeline used to update particles.
class ComputePipeline {
    public:
        /// Create new compute pipeline.
        /**
         * @param device Logical Vulkan device.
         */
        ComputePipeline(VkDevice device);
        
        /// Destructor.
        ~ComputePipeline();
        
    private:
        Shader shader;
};
