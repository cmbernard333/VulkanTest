#include "HelloTriangleApplication.hpp"
#define GLFW_INCLUDE_VULKAN // force GLFW to include vulkan header
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>
#include <functional>

/* public */
void HelloTriangleApplication::Run()
{
	this->InitWindow();
	this->InitVulkan();
	this->MainLoop();
	this->Cleanup();
}

/* private */
void HelloTriangleApplication::InitWindow()
{
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); // TODO: handle resizing later

	this->window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
}
void HelloTriangleApplication::InitVulkan()
{

}
void HelloTriangleApplication::MainLoop()
{
	while (!glfwWindowShouldClose(this->window))
	{
		glfwPollEvents();
	}
}
void HelloTriangleApplication::Cleanup()
{
	// TODO: use RAII instead (destructor)
	glfwDestroyWindow(this->window);
	glfwTerminate();
}