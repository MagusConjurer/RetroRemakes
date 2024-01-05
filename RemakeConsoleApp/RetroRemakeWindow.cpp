#include <stdio.h>
#include <stdexcept>
#include <vector>
#include <map>
#include <set>

#include "RetroRemakeWindow.h"

using std::runtime_error;
using std::vector;
using std::multimap;
using std::set;
using std::make_pair;

RetroRemakeWindow::RetroRemakeWindow() {

}

void RetroRemakeWindow::run() {
    initWindow();
	initVulkan();
	mainLoop();
	cleanup();
}

void RetroRemakeWindow::initWindow() {
    // Initialize GLFW and handle failure
    if (!glfwInit()) {
        glfwTerminate();
        throw runtime_error("GLFW initialization failed!");
    }

    // Window Properties
    glfwWindowHint(GLFW_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Retro Remakes", NULL, NULL);
    if (!mainWindow) {
        glfwTerminate();
        throw runtime_error("Failed to create GLFW window!");
    }

    // Buffer size info
    glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);
}

void RetroRemakeWindow::initVulkan() {
    if (!glfwVulkanSupported()) {
        glfwTerminate();
        glfwDestroyWindow(mainWindow);
        throw runtime_error("GLFW with Vulkan not supported!");
    }

    createInstance();
    // setupDebugMessenger();
    createSurface();
    pickPhysicalDevice();
    createLogicalDevice();
    // createSwapChain();
    // createImageViews();
    // createRenderPass();
    // createGraphicsPipeline();
    // createFramebuffers();
    // createCommandPool();
    // createCommandBuffer();
    // createSyncObjects();
}

void RetroRemakeWindow::createInstance() {
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Retro Remakes";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;

    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    createInfo.enabledExtensionCount = glfwExtensionCount;
    createInfo.ppEnabledExtensionNames = glfwExtensions;
    createInfo.enabledLayerCount = 0;

    if (vkCreateInstance(&createInfo, nullptr, &instance)) {
        throw runtime_error("Failed to create Vulkan instance!");
    }
}

void RetroRemakeWindow::createSurface() {
    if (glfwCreateWindowSurface(instance, mainWindow, nullptr, &surface) != VK_SUCCESS) {
        throw runtime_error("Failed to create window");
    }
}

void RetroRemakeWindow::pickPhysicalDevice() {
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

    if (deviceCount == 0) {
        throw runtime_error("No GPUs with Vulkan support found!");
    }

    vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

    for (const auto& device : devices) {
        if (isDeviceSuitable(device)) {
            physicalDevice = device;
            break;
        }
    }

    if (physicalDevice == VK_NULL_HANDLE) {
        throw runtime_error("Failed to find suitable GPU!");
    }
}

bool RetroRemakeWindow::isDeviceSuitable(VkPhysicalDevice device) {
    QueueFamilyIndices indices = findQueueFamilies(device);

    return indices.isComplete();
}

QueueFamilyIndices RetroRemakeWindow::findQueueFamilies(VkPhysicalDevice device) {
    QueueFamilyIndices indices;

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
    vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

    int i = 0;
    for (const auto& queueFamily : queueFamilies) {
        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            indices.graphicsFamily = i;
        }

        if (indices.isComplete()) {
            break;
        }

        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);

        if (presentSupport) {
            indices.presentFamily = i;
        }

        i++;
    }

    return indices;
}

void RetroRemakeWindow::createLogicalDevice() {
    QueueFamilyIndices indices = findQueueFamilies(physicalDevice);

    vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    set<uint32_t> uniqueueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };

    float queuePriority = 1.0f;
    for (uint32_t queueFamily : uniqueueQueueFamilies) {
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.push_back(queueCreateInfo);
    }

    vkGetDeviceQueue(device, indices.presentFamily.value(), 0, &presentQueue);

    VkPhysicalDeviceFeatures deviceFeatures{};
    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    createInfo.pQueueCreateInfos = queueCreateInfos.data();
    createInfo.queueCreateInfoCount = 1;
    createInfo.pEnabledFeatures = &deviceFeatures;
    createInfo.enabledExtensionCount = 0;
    // No validation layers setup
    createInfo.enabledLayerCount = 0;

    if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS) {
        throw runtime_error("Failed to create logical device!");
    }

    vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &graphicsQueue);
}

void RetroRemakeWindow::createSwapChain() {}

void RetroRemakeWindow::createImageViews() {}

void RetroRemakeWindow::createRenderPass() {}

void RetroRemakeWindow::createGraphicsPipeline() {}

void RetroRemakeWindow::createFramebuffers() {}

void RetroRemakeWindow::createCommandPool() {}

void RetroRemakeWindow::createCommandBuffer() {}

void RetroRemakeWindow::createSyncObjects() {}

void RetroRemakeWindow::mainLoop() {
    // Loop until window closed
    while (!glfwWindowShouldClose(mainWindow)) {
        // Handle user input
        glfwPollEvents();
        drawFrame();
    }
}

void RetroRemakeWindow::drawFrame() {

}

void RetroRemakeWindow::cleanup() {
    vkDestroyDevice(device, nullptr);
    vkDestroySurfaceKHR(instance, surface, nullptr);
    vkDestroyInstance(instance, nullptr);

    glfwDestroyWindow(mainWindow);
    glfwTerminate();
}
