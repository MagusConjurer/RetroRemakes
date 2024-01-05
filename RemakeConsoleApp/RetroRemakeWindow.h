#pragma once

#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#include <vulkan/vulkan.h>
#include <optional>

struct QueueFamilyIndices { 
	std::optional<uint32_t> graphicsFamily; 
	std::optional<uint32_t> presentFamily;

	bool isComplete() {
		return graphicsFamily.has_value() && presentFamily.has_value();
	}
};

class RetroRemakeWindow {
public:
	RetroRemakeWindow();
	void run();
private:
	// GLFW window details
	const GLint WIDTH = 1080;
	const GLint HEIGHT = 720;
	int bufferWidth;
	int bufferHeight;
	GLFWwindow* mainWindow;

	// Vulkan details
	VkInstance instance;
	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
	VkDevice device;
	VkQueue graphicsQueue;
	VkQueue presentQueue;
	VkSurfaceKHR surface;

	void initWindow();
	void initVulkan();
	void createInstance();
	void createSurface();
	void pickPhysicalDevice();
	bool isDeviceSuitable(VkPhysicalDevice device);
	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
	void createLogicalDevice();
	void createSwapChain();
	void createImageViews();
	void createRenderPass();
	void createGraphicsPipeline();
	void createFramebuffers();
	void createCommandPool();
	void createCommandBuffer();
	void createSyncObjects();

	void mainLoop();
	void drawFrame();
	void cleanup();
};

