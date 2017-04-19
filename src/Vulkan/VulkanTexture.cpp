#include "VulkanTexture.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <iostream>

VulkanTexture::VulkanTexture(const char* data, unsigned int length) {
    // Load texture from memory.
    int width, height, channels;
    stbi_uc* pixels = stbi_load_from_memory(reinterpret_cast<const unsigned char*>(data), length, &width, &height, &channels, 0);
    if (pixels == nullptr) {
        std::cerr << "Failed to load image from memory." << std::endl;
        exit(-1);
    }
    
    // Clean up.
    stbi_image_free(pixels);
}

VulkanTexture::~VulkanTexture() {
    
}
