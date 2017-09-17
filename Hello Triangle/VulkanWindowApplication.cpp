#include "VulkanWindowApplication.hpp"
#define GLFW_INCLUDE_VULKAN // force GLFW to include vulkan header
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>
#include <functional>

/* public */
void VulkanWindowApplication::Run()
{
	this->InitWindow();
	this->InitVulkan();
	this->MainLoop();
	this->Cleanup();
}

/* private */
void VulkanWindowApplication::InitWindow()
{
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); // TODO: handle resizing later

	this->GlfwWindow = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
}
void VulkanWindowApplication::InitVulkan()
{
	this->CreateVulkanInstance();
}
void VulkanWindowApplication::CreateVulkanInstance()
{
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions = nullptr;

	VkApplicationInfo AppInfo = {};
	VkInstanceCreateInfo CreateInfo = {};
	VkResult result;

	// application information
	AppInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	AppInfo.pApplicationName = (const char*) "Hello Triangle";
	AppInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	AppInfo.pEngineName = "No Engine";
	AppInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	AppInfo.apiVersion = VK_API_VERSION_1_0;

	// retrieve the required extensions to interface with window system
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	// instance creation information
	CreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	CreateInfo.pApplicationInfo = &AppInfo;
	CreateInfo.enabledExtensionCount = glfwExtensionCount;
	CreateInfo.ppEnabledExtensionNames = glfwExtensions;
	CreateInfo.enabledLayerCount = 0; // TODO: global validation layer enablement

	// TODO: for fun and enjoyment print the list of extensions supported
	// TODO: compare the list of extensions returned by `glfwGetRequiredInstanceExtensions` and GetSupporteExtensions()
	std::cout << "Available Vulkan Extensions:" << std::endl;
	for (const auto& Extension : this->GetSupportedExtensions())
	{
		static uint32_t Counter = 0;
		std::cout << ++Counter << "\t" << Extension.extensionName << std::endl;
	}

	result = vkCreateInstance(&CreateInfo, nullptr, &(this->VInstance));

	// throw a runtime exception if the instance wasn't succesfully created
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create vulkan instance!");
	}
	
}
void VulkanWindowApplication::MainLoop()
{
	while (!glfwWindowShouldClose(this->GlfwWindow))
	{
		glfwPollEvents();
	}
}
void VulkanWindowApplication::Cleanup()
{
	// TODO: use RAII instead (destructor)
	// TOOD: order matters here
	glfwDestroyWindow(this->GlfwWindow);

	vkDestroyInstance(this->VInstance, nullptr);

	glfwTerminate();
}


// Note: C++ 11 std::vector<T> has std::move<T> semantics
std::vector<VkExtensionProperties> VulkanWindowApplication::GetSupportedExtensions()
{
	uint32_t ExtensionCount = 0;
	std::vector<VkExtensionProperties> SupportedExtensions;
	vkEnumerateInstanceExtensionProperties(nullptr, &ExtensionCount, SupportedExtensions.data());
	return SupportedExtensions;
}
