#pragma once
#include <GLFW/glfw3.h>

constexpr int WIDTH = 800;
constexpr int HEIGHT = 600;

class HelloTriangleApplication {
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

	// Window pointer
	GLFWwindow* window;
};