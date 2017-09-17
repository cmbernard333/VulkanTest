#pragma once
#define GLFW_INCLUDE_VULKAN // force GLFW to include vulkan header
#include <GLFW/glfw3.h>
#include <vector>

constexpr int WIDTH = 800;
constexpr int HEIGHT = 600;

class VulkanWindowApplication {
public:
	void Run();
private:
	/**
	 * Initialize the window to display stuff
	 */
	void InitWindow();
	/**
	 * Initialize the Vulkan instance and associated resources
	 */
	void InitVulkan();
	/*
	 * Create the Vulkan instance
	 */
	void CreateVulkanInstance();
	/**
	 * Run the main game loop
	 * Render frames
	 * Iterate until the window is closed
	 */
	void MainLoop();
	/**
	 * Cleanup all resources
	 */
	void Cleanup();

	std::vector<VkExtensionProperties> GetSupportedExtensions();

	// Window pointer
	GLFWwindow* GlfwWindow;
	// Vulkan Instance
	VkInstance VInstance;
};