#define VK_PROTOTYPES
#define VK_USE_PLATFORM_WIN32_KHR
#define NOMINMAX

#include "Renderer.hpp"

#include <set>
#include <iostream>
#include <limits>
#include <algorithm>
#include "Texture.hpp"
#include "GraphicsPipeline.hpp"
#include "ComputePipeline.hpp"
#include "StorageBuffer.hpp"
#include "UniformBuffer.hpp"
#include <glm/glm.hpp>
#include <random>

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#ifndef NDEBUG
static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objType, uint64_t obj, size_t location, int32_t code, const char* layerPrefix, const char* msg, void* userData) {
    std::cerr << "Validation layer: " << msg << std::endl;
    
    return VK_FALSE;
}
#endif

Renderer::Renderer(Window& window) {
    // Create Vulkan instance.
    createInstance();
    setupDebugCallback();
    
    // Create window.
    window.setWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    GLFWwindow* windowStruct = window.createWindow("Vulkan");
    
    // Create surface.
    createSurface(windowStruct);
    
    // Create logical device.
    createDevice();
    
    // Create swap chain.
    VkFormat format = createSwapChain(window.getWidth(), window.getHeight());
    createImageViews(format);
    
    // Create render pass.
    createRenderPass(format);
    
    // Create frame buffers.
    createFramebuffers();
    
    // Create command buffers.
    createCommandPools();
    createCommandBuffers();
    
    // Create descriptor pool.
    createDescriptorPool();
    
    // Create semaphores.
    createSemaphores();
    
    // Create fences.
    createFences();
    
    // Create pipelines.
    graphicsPipeline = new GraphicsPipeline(device, swapChainExtent, renderPass);
    computePipeline = new ComputePipeline(device);
    
    // Create buffers.
    std::mt19937 randomEngine;
    std::uniform_real_distribution<float> distribution(-3.0f, 3.0f);
    
    Particle particles[particleCount];
    for (int i=0; i < particleCount; ++i) {
        particles[i].position = glm::vec4(distribution(randomEngine), distribution(randomEngine), distribution(randomEngine), 1.0f);
        particles[i].velocity = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
    }
    
    particleBuffer[0] = new StorageBuffer(particles, sizeof(particles), device, physicalDevice, descriptorPool, graphicsQueue, graphicsCommandPool);
    particleBuffer[1] = new StorageBuffer(particles, sizeof(particles), device, physicalDevice, descriptorPool, graphicsQueue, graphicsCommandPool);

    CameraUniform cameraUniform;
    cameraUniform.viewProjectionMatrix = camera.getViewProjectionMatrix(glm::vec2(window.getWidth(), window.getHeight()));
    cameraUniform.cameraPosition = glm::vec4(camera.getPosition(), 1.0f);
    cameraUniform.cameraUp = glm::vec4(camera.getUp(), 1.0f);
    cameraBuffer = new UniformBuffer(&cameraUniform, sizeof(cameraUniform), device, physicalDevice, descriptorPool, VK_SHADER_STAGE_GEOMETRY_BIT);
    
    UpdateUniform updateUniform;
    updateUniform.deltaTime = 0.0f;
    updateUniform.particleCount = particleCount;
    updateBuffer = new UniformBuffer(&updateUniform, sizeof(updateUniform), device, physicalDevice, descriptorPool, VK_SHADER_STAGE_COMPUTE_BIT);
}

Renderer::~Renderer() {
    delete updateBuffer;
    delete cameraBuffer;
    delete particleBuffer[0];
    delete particleBuffer[1];
    delete graphicsPipeline;
    delete computePipeline;
    delete particleTexture;
    
    vkDestroyFence(device, graphicsFence, nullptr);
    vkDestroyFence(device, computeFence, nullptr);
    vkDestroySemaphore(device, imageAvailableSemaphore, nullptr);
    vkDestroySemaphore(device, renderFinishedSemaphore, nullptr);
    
    vkDestroyDescriptorPool(device, descriptorPool, nullptr);
    
    vkDestroyCommandPool(device, graphicsCommandPool, nullptr);
    if (graphicsCommandPool != computeCommandPool)
        vkDestroyCommandPool(device, computeCommandPool, nullptr);
    
    for (VkFramebuffer framebuffer : swapChainFramebuffers)
        vkDestroyFramebuffer(device, framebuffer, nullptr);
    
    vkDestroyRenderPass(device, renderPass, nullptr);
    
    for (VkImageView imageView : swapChainImageViews)
        vkDestroyImageView(device, imageView, nullptr);
    vkDestroySwapchainKHR(device, swapChain, nullptr);
    
    vkDestroyDevice(device, nullptr);
    
    vkDestroySurfaceKHR(instance, surface, nullptr);
    
#ifndef NDEBUG
    auto DestroyDebugReportCallback = (PFN_vkDestroyDebugReportCallbackEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugReportCallbackEXT");
    if (DestroyDebugReportCallback != nullptr)
        DestroyDebugReportCallback(instance, callback, nullptr);
#endif
    
    vkDestroyInstance(instance, nullptr);
}

void Renderer::setTexture(const char* textureData, unsigned int dataLength) {
    particleTexture = new Texture(textureData, dataLength, device, physicalDevice, graphicsCommandPool, graphicsQueue, descriptorPool);
}

void Renderer::recordCommandBuffers() {
    recordUpdateCommandBuffer(0);
    recordUpdateCommandBuffer(1);
    
    recordRenderCommandBuffer(0);
    recordRenderCommandBuffer(1);
}

void Renderer::frame(float deltaTime) {
    
    // Render boids.
    render();
    
    // Wait for finished rendering.
    waitFence(graphicsFence);
    
    // Update boids.
    update(deltaTime);
    
    // Wait for finished computing.
    waitFence(computeFence);
    
    // Swap particle buffers.
    bufferIndex = 1 - bufferIndex;
}

void Renderer::createInstance() {
    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Particle Simulator";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;
    
    std::vector<const char*> extensions;
    extensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
    extensions.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
    
    std::vector<const char*> validationLayers;
#ifndef NDEBUG
    extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
    validationLayers.push_back("VK_LAYER_LUNARG_standard_validation");
#endif
    
    VkInstanceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    createInfo.enabledExtensionCount = extensions.size();
    createInfo.ppEnabledExtensionNames = extensions.data();
    createInfo.enabledLayerCount = validationLayers.size();
    createInfo.ppEnabledLayerNames = validationLayers.data();
    
    if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
        std::cerr << "Failed to create instance." << std::endl;
        exit(-1);
    }
}

void Renderer::setupDebugCallback() {
#ifndef NDEBUG
    VkDebugReportCallbackCreateInfoEXT createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
    createInfo.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;
    createInfo.pfnCallback = debugCallback;
    
    auto CreateDebugReportCallbackEXT = (PFN_vkCreateDebugReportCallbackEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugReportCallbackEXT");
    if (CreateDebugReportCallbackEXT == nullptr)
        std::cerr << "Failed to get CreateDebugReportCallbackEXT function" << std::endl;
    
    if (CreateDebugReportCallbackEXT(instance, &createInfo, nullptr, &callback) != VK_SUCCESS)
        std::cerr << "Failed to set up debug callback" << std::endl;
#endif
}

void Renderer::createDevice() {
    // Find physical device to use.
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
    
    if (deviceCount == 0) {
        std::cerr << "Failed to find GPU with Vulkan support." << std::endl;
        exit(-1);
    }
    
    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());
    
    for (VkPhysicalDevice device : devices){
        VkPhysicalDeviceProperties deviceProperties;
        VkPhysicalDeviceFeatures deviceFeatures;
        vkGetPhysicalDeviceProperties(device, &deviceProperties);
        vkGetPhysicalDeviceFeatures(device, &deviceFeatures);
        
        // Checking for discrete (dedicated) GPU.
        /// @todo Check for actually necessary GPU features.
        // Maybe: Wrap this in a function that takes as argument the things we are looking for.
        if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU){
            std::cout << "Found suitable GPU." << std::endl;
            physicalDevice = device;
            break;
        }
    }
    
    if (physicalDevice == VK_NULL_HANDLE)
        std::cerr << "Failed to find suitable GPU's." << std::endl;
    
    // Find queue families.
    graphicsFamily = -1;
    computeFamily = -1;
    int presentFamily = -1;
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);
    
    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());
    
    // Check for available queue families.
    int i = 0;
    for (const VkQueueFamilyProperties& queueFamily : queueFamilies){
        if (queueFamily.queueCount > 0) {
            if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
                graphicsFamily = i;
            
            if (queueFamily.queueFlags & VK_QUEUE_COMPUTE_BIT)
                computeFamily = i;
            
            VkBool32 presentSupport = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, surface, &presentSupport);
            if (presentSupport)
                presentFamily = i;
        }
        
        if (graphicsFamily >= 0 && presentFamily >= 0 && computeFamily >= 0)
            break;
        ++i;
    }
    
    std::cout << "Graphics family: " << graphicsFamily << std::endl;
    std::cout << "Compute family: " << computeFamily << std::endl;
    
    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<int> uniqueQueueFamilies = { graphicsFamily, presentFamily };
    
    // Queue priority between 0-1.
    float queuePriority = 1.0f;
    for (int queueFamily : uniqueQueueFamilies) {
        VkDeviceQueueCreateInfo queueCreateInfo = {};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.push_back(queueCreateInfo);
    }
    
    // Device extensions.
    std::vector<const char*> deviceExtensions;
    deviceExtensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
    
    /// @todo Check if the extensions are actually supported.
    
    // Device features.
    VkPhysicalDeviceFeatures deviceFeatures = {};
    deviceFeatures.geometryShader = VK_TRUE;
    
    VkDeviceCreateInfo deviceCreateInfo = {};
    deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceCreateInfo.queueCreateInfoCount = queueCreateInfos.size();
    deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();
    deviceCreateInfo.pEnabledFeatures = &deviceFeatures;
    deviceCreateInfo.enabledExtensionCount = deviceExtensions.size();
    deviceCreateInfo.ppEnabledExtensionNames = deviceExtensions.data();
    
    if (vkCreateDevice(physicalDevice, &deviceCreateInfo, nullptr, &device) != VK_SUCCESS)
        std::cerr << "Could not create logical device." << std::endl;
    else
        std::cout << "Logical device created." << std::endl;
    
    vkGetDeviceQueue(device, graphicsFamily, 0, &graphicsQueue);
    vkGetDeviceQueue(device, computeFamily, 0, &computeQueue);
    vkGetDeviceQueue(device, presentFamily, 0, &presentQueue);
}

void Renderer::createSurface(GLFWwindow* window) {
    VkWin32SurfaceCreateInfoKHR surfaceInfo = {};
    surfaceInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    surfaceInfo.hwnd = glfwGetWin32Window(window);
    surfaceInfo.hinstance = GetModuleHandle(nullptr);
    
    if (vkCreateWin32SurfaceKHR(instance, &surfaceInfo, nullptr, &surface) != VK_SUCCESS) {
        std::cerr << "Failed to create surface." << std::endl;
        exit(-1);
    }
}

VkFormat Renderer::createSwapChain(unsigned int width, unsigned int height) {
    // Determine swap chain support.
    SwapChainSupport swapChainSupport = querySwapChainSupport();
    
    VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
    VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
    swapChainExtent = chooseSwapExtent(swapChainSupport.capabilities, width, height);
    
    uint32_t imageCount = swapChainSupport.capabilities.minImageCount;
    
    VkSwapchainCreateInfoKHR swapChainCreateInfo = {};
    swapChainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapChainCreateInfo.surface = surface;
    swapChainCreateInfo.minImageCount = imageCount;
    swapChainCreateInfo.imageFormat = surfaceFormat.format;
    swapChainCreateInfo.imageColorSpace = surfaceFormat.colorSpace;
    swapChainCreateInfo.imageExtent = swapChainExtent;
    swapChainCreateInfo.imageArrayLayers = 1;
    swapChainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    
    /// @todo Handle case where graphics queue is not the same as the present queue.
    swapChainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    swapChainCreateInfo.queueFamilyIndexCount = 0;
    swapChainCreateInfo.pQueueFamilyIndices = nullptr;
    
    swapChainCreateInfo.preTransform = swapChainSupport.capabilities.currentTransform;
    swapChainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    swapChainCreateInfo.presentMode = presentMode;
    swapChainCreateInfo.clipped = VK_TRUE;
    swapChainCreateInfo.oldSwapchain = VK_NULL_HANDLE;
    
    if (vkCreateSwapchainKHR(device, &swapChainCreateInfo, nullptr, &swapChain) != VK_SUCCESS) {
        std::cerr << "Failed to create swap chain." << std::endl;
        exit(-1);
    }
    
    // Get swap chain images.
    vkGetSwapchainImagesKHR(device, swapChain, &imageCount, nullptr);
    swapChainImages.resize(imageCount);
    vkGetSwapchainImagesKHR(device, swapChain, &imageCount, swapChainImages.data());
    
    return surfaceFormat.format;
}

Renderer::SwapChainSupport Renderer::querySwapChainSupport() {
    SwapChainSupport swapChainSupport;
    
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &swapChainSupport.capabilities);
    
    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, nullptr);
    swapChainSupport.formats.resize(formatCount);
    vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, swapChainSupport.formats.data());
    
    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, nullptr);
    swapChainSupport.presentModes.resize(presentModeCount);
    vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, swapChainSupport.presentModes.data());
    
    return swapChainSupport;
}

VkSurfaceFormatKHR Renderer::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) {
    if (availableFormats.size() == 1 && availableFormats[0].format == VK_FORMAT_UNDEFINED)
        return {VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR};
    
    for (const VkSurfaceFormatKHR& availableFormat : availableFormats) {
        if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
            return availableFormat;
    }
    
    return availableFormats[0];
}

VkPresentModeKHR Renderer::chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) {
    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D Renderer::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, unsigned int width, unsigned int height) {
    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
        return capabilities.currentExtent;
    } else {
        VkExtent2D actualExtent = {width, height};
        actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
        actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));
        
        return actualExtent;
    }
}

void Renderer::createImageViews(VkFormat format) {
    swapChainImageViews.resize(swapChainImages.size());
    
    for (uint32_t i = 0; i < swapChainImages.size(); ++i) {
        VkImageViewCreateInfo imageViewCreateInfo = {};
        imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        imageViewCreateInfo.image = swapChainImages[i];
        imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        imageViewCreateInfo.format = format;
        imageViewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        imageViewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        imageViewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        imageViewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        imageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
        imageViewCreateInfo.subresourceRange.levelCount = 1;
        imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
        imageViewCreateInfo.subresourceRange.layerCount = 1;
        
        if (vkCreateImageView(device, &imageViewCreateInfo, nullptr, &swapChainImageViews[i])) {
            std::cerr << "Failed to create image views." << std::endl;
            exit(-1);
        }
    }
}

void Renderer::createRenderPass(VkFormat format) {
    VkSubpassDependency dependency = {};
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.srcAccessMask  = 0;
    dependency.dstSubpass = 0;
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    
    // We use a single color buffer.
    VkAttachmentDescription colorAttachment = {};
    colorAttachment.format = format;
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    
    // Subpasses.
    VkAttachmentReference colorAttachmentRef = {};
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    
    VkSubpassDescription subpass = {};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;
    subpass.pDepthStencilAttachment = nullptr;
    
    // Create render pass.
    VkRenderPassCreateInfo renderPassInfo = {};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = 1;
    renderPassInfo.pAttachments = &colorAttachment;
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;
    renderPassInfo.dependencyCount = 1;
    renderPassInfo.pDependencies = &dependency;
    
    if (vkCreateRenderPass(device, &renderPassInfo, nullptr, &renderPass) != VK_SUCCESS) {
        std::cerr << "Failed to create render pass" << std::endl;
        exit(-1);
    }
}

void Renderer::createFramebuffers() {
    swapChainFramebuffers.resize(swapChainImageViews.size());
    
    for (std::size_t i = 0; i < swapChainImageViews.size(); ++i) {
        VkImageView attachments[] = {swapChainImageViews[i]};
        
        VkFramebufferCreateInfo framebufferInfo = {};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = renderPass;
        framebufferInfo.attachmentCount = 1;
        framebufferInfo.pAttachments = attachments;
        framebufferInfo.width = swapChainExtent.width;
        framebufferInfo.height = swapChainExtent.height;
        framebufferInfo.layers = 1;
        
        if (vkCreateFramebuffer(device, &framebufferInfo, nullptr, &swapChainFramebuffers[i]) != VK_SUCCESS) {
            std::cerr << "Failed to create framebuffer" << std::endl;
            exit(-1);
        }
    }
}

void Renderer::createCommandPools() {
    VkCommandPoolCreateInfo poolInfo = {};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.queueFamilyIndex = graphicsFamily;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    
    if (vkCreateCommandPool(device, &poolInfo, nullptr, &graphicsCommandPool) != VK_SUCCESS) {
        std::cerr << "Failed to create graphics command pool" << std::endl;
        exit(-1);
    }
    
    if (graphicsFamily == computeFamily) {
        computeCommandPool = graphicsCommandPool;
    } else {
        poolInfo.queueFamilyIndex = computeFamily;
        
        if (vkCreateCommandPool(device, &poolInfo, nullptr, &computeCommandPool) != VK_SUCCESS) {
            std::cerr << "Failed to create compute command pool" << std::endl;
            exit(-1);
        }
    }
}

void Renderer::createCommandBuffers() {
    // Graphics command buffer.
    VkCommandBufferAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = graphicsCommandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = 2;
    
    if (vkAllocateCommandBuffers(device, &allocInfo, graphicsCommandBuffers) != VK_SUCCESS) {
        std::cerr << "Failed to allocate graphics command buffer!" << std::endl;
        exit(-1);
    }
    
    // Compute command buffer.
    allocInfo.commandPool = computeCommandPool;
    
    if (vkAllocateCommandBuffers(device, &allocInfo, computeCommandBuffers) != VK_SUCCESS) {
        std::cerr << "Failed to allocate compute command buffer!" << std::endl;
        exit(-1);
    }
}

void Renderer::createDescriptorPool() {
    VkDescriptorPoolSize poolSizes[3];
    
    // Storage buffers.
    poolSizes[0] = {};
    poolSizes[0].type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    poolSizes[0].descriptorCount = 2;
    
    // Uniform buffers.
    poolSizes[1] = {};
    poolSizes[1].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    poolSizes[1].descriptorCount = 2;
    
    // Samplers.
    poolSizes[2] = {};
    poolSizes[2].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    poolSizes[2].descriptorCount = 1;
    
    // Create descriptor pool.
    VkDescriptorPoolCreateInfo poolInfo = {};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = 3;
    poolInfo.pPoolSizes = poolSizes;
    poolInfo.maxSets = 5;
    
    if (vkCreateDescriptorPool(device, &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
        std::cerr << "Failed to create descriptor pool." << std::endl;
        exit(-1);
    }
}

void Renderer::createSemaphores() {
    VkSemaphoreCreateInfo semaphoreInfo = {};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    
    if (vkCreateSemaphore(device, &semaphoreInfo, nullptr, &imageAvailableSemaphore) != VK_SUCCESS)
        std::cout << "Couldn't create semaphore" << std::endl;
    
    if (vkCreateSemaphore(device, &semaphoreInfo, nullptr, &renderFinishedSemaphore) != VK_SUCCESS)
        std::cout << "Couldn't create semaphore" << std::endl;
}

void Renderer::createFences() {
    VkFenceCreateInfo fenceInfo = {};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.pNext = nullptr;
    fenceInfo.flags = 0;
    
    vkCreateFence(device, &fenceInfo, nullptr, &graphicsFence);
    vkCreateFence(device, &fenceInfo, nullptr, &computeFence);
}

void Renderer::recordUpdateCommandBuffer(int frame) {
    // Start command buffer recording.
    VkCommandBufferBeginInfo beginInfo = {};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = 0;
    beginInfo.pInheritanceInfo = nullptr;
    
    vkBeginCommandBuffer(computeCommandBuffers[frame], &beginInfo);
    
    // Update particles.
    vkCmdBindPipeline(computeCommandBuffers[frame], VK_PIPELINE_BIND_POINT_COMPUTE, computePipeline->getPipeline());
    
    std::vector<VkDescriptorSet> descriptorSets;
    descriptorSets.push_back(particleBuffer[1-frame]->getDescriptorSet());
    descriptorSets.push_back(particleBuffer[frame]->getDescriptorSet());
    descriptorSets.push_back(updateBuffer->getDescriptorSet());
    vkCmdBindDescriptorSets(computeCommandBuffers[frame], VK_PIPELINE_BIND_POINT_COMPUTE, computePipeline->getPipelineLayout(), 0, descriptorSets.size(), descriptorSets.data(), 0, nullptr);
    int groupSize = 256;
    vkCmdDispatch(computeCommandBuffers[frame], particleCount / groupSize + (particleCount % groupSize != 0), 1, 1);
    
    if (vkEndCommandBuffer(computeCommandBuffers[frame]) != VK_SUCCESS) {
        std::cerr << "Failed to record command buffer" << std::endl;
        exit(-1);
    }
}

void Renderer::recordRenderCommandBuffer(int frame) {
    // Start command buffer recording.
    VkCommandBufferBeginInfo beginInfo = {};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = 0;
    beginInfo.pInheritanceInfo = nullptr;
    
    vkBeginCommandBuffer(graphicsCommandBuffers[frame], &beginInfo);
    
    // Start render pass.
    VkRenderPassBeginInfo renderPassInfo = {};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = renderPass;
    renderPassInfo.framebuffer = swapChainFramebuffers[frame];
    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = swapChainExtent;
    VkClearValue clearValues = {};
    clearValues.color = {0.0f, 0.0f, 0.0f, 0.0f};
    renderPassInfo.clearValueCount = 1;
    renderPassInfo.pClearValues = &clearValues;
    
    vkCmdBeginRenderPass(graphicsCommandBuffers[frame], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
    
    // Render particles.
    vkCmdBindPipeline(graphicsCommandBuffers[frame], VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline->getPipeline());
    
    std::vector<VkDescriptorSet> descriptorSets;
    descriptorSets.push_back(particleBuffer[frame]->getDescriptorSet());
    descriptorSets.push_back(cameraBuffer->getDescriptorSet());
    descriptorSets.push_back(particleTexture->getDescriptorSet());
    vkCmdBindDescriptorSets(graphicsCommandBuffers[frame], VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline->getPipelineLayout(), 0, descriptorSets.size(), descriptorSets.data(), 0, nullptr);
    vkCmdDraw(graphicsCommandBuffers[frame], particleCount, 1, 0, 0);
    
    // End render pass.
    vkCmdEndRenderPass(graphicsCommandBuffers[frame]);
    
    if (vkEndCommandBuffer(graphicsCommandBuffers[frame]) != VK_SUCCESS) {
        std::cerr << "Failed to record command buffer" << std::endl;
        exit(-1);
    }
}

void Renderer::update(float deltaTime) {
    // Update buffer.
    UpdateUniform updateUniform;
    updateUniform.deltaTime = deltaTime;
    updateUniform.particleCount = particleCount;
    updateBuffer->setData(&updateUniform, sizeof(updateUniform));
    
    // Create submit info.
    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &computeCommandBuffers[bufferIndex];
    
    if (vkQueueSubmit(computeQueue, 1, &submitInfo, computeFence) != VK_SUCCESS)
        std::cout << "Could not submit command buffer to compute queue." << std::endl;
}

void Renderer::render() {
    // Get image from swapchain.
    vkAcquireNextImageKHR(device, swapChain, std::numeric_limits<uint64_t>::max(), imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);
    
    // Create submit info.
    VkSubmitInfo submitInfo = {};
    VkSemaphore waitSemaphores[] = {imageAvailableSemaphore};
    VkSemaphore signalSemaphores[] = {renderFinishedSemaphore};
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &graphicsCommandBuffers[bufferIndex];
    
    if (vkQueueSubmit(graphicsQueue, 1, &submitInfo, graphicsFence) != VK_SUCCESS)
        std::cout << "Could not submit command buffer to graphics queue." << std::endl;
    
    // Setup presentation
    VkPresentInfoKHR presentInfo = {};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = &swapChain;
    presentInfo.pImageIndices = &imageIndex;
    presentInfo.pResults = nullptr;
    
    // Submit presentation request.
    vkQueuePresentKHR(presentQueue, &presentInfo);
}

void Renderer::waitFence(VkFence& fence) {
    while (vkWaitForFences(device, 1, &fence, VK_TRUE, 1000) != VK_SUCCESS);
    vkResetFences(device, 1, &fence);
}
