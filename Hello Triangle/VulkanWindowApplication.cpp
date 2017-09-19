#include "VulkanWindowApplication.hpp"
#define GLFW_INCLUDE_VULKAN // force GLFW to include vulkan header
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>
#include <functional>
#include <cstring>

/* proxy functions */

VkResult CreateDebugReportCallbackEXT(VkInstance Instance, const VkDebugReportCallbackCreateInfoEXT* pCreateInfo,
	const VkAllocationCallbacks* pAllocator, VkDebugReportCallbackEXT* pCallback)
{
	auto func = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(Instance, VK_CREATE_DEBUG_REPORT_CALLBACK_EXT_FNAME);
	if (func != nullptr)
	{
		return func(Instance, pCreateInfo, pAllocator, pCallback);
	}
	else
	{
		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}
}

void DestroyDebugReportCallbackEXT(VkInstance Instance, VkDebugReportCallbackEXT Callback, const VkAllocationCallbacks* pAllocator)
{
	auto func = (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(Instance, VK_DESTROY_DEBUG_REPORT_CALLBACK_EXT_FNAME);
	if (func != nullptr)
	{
		func(Instance, Callback, pAllocator);
	}
}

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
	this->SetupDebugCallback();
}
bool VulkanWindowApplication::CheckValidationLayerSupport(const char* const* LayerNames, std::size_t LayerNamesCount)
{
	uint32_t LayerCount;
	std::vector<VkLayerProperties> AvailableLayers;
	const char* LayerName = nullptr;

	vkEnumerateInstanceLayerProperties(&LayerCount, nullptr);
	AvailableLayers.resize(LayerCount);
	vkEnumerateInstanceLayerProperties(&LayerCount, AvailableLayers.data());

	// check if all the layers in passed in LayerNames are available on this platform
	for (std::size_t i = 0; i < LayerNamesCount; i++)
	{
		LayerName = LayerNames[i];
		bool bLayerFound = false;
		for (const auto& LayerProperties : AvailableLayers)
		{
			if (strcmp(LayerName, LayerProperties.layerName) == 0)
			{
				bLayerFound = true;
				break;
			}
		}

		if (!bLayerFound)
		{
			return false;
		}
	}

	std::cout << "All Validation Layers Supported." << std::endl;

	return true;
}
void VulkanWindowApplication::SetupDebugCallback()
{
	VkDebugReportCallbackCreateInfoEXT CallbackCreateInfo = {};
	if (!gbEnableValidationLayers)
	{
		return;
	}
	CallbackCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
	// filters the types of messages you would like to receive
	CallbackCreateInfo.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;
	CallbackCreateInfo.pfnCallback = VulkanWindowApplication::DebugCallback;
	// optional : set the userData field to a pointer to the class itself
	// CallbackCreateInfo.userData = this;
	
	// use the proxy function to create a debug callback given the VkDebugReportCallbackCreateInfoEXT struct
	if (VK_SUCCESS != CreateDebugReportCallbackEXT(this->VInstance, &CallbackCreateInfo, nullptr, &this->VDebugReportCallback))
	{
		throw std::runtime_error("Failed to set up debug callback!");
	}


}
void VulkanWindowApplication::CreateVulkanInstance()
{
	std::vector<const char*> glfwExtensions;

	VkApplicationInfo AppInfo = {};
	VkInstanceCreateInfo CreateInfo = {};
	VkResult result;

	if (gbEnableValidationLayers && !CheckValidationLayerSupport())
	{
		throw std::runtime_error("Validation layers requested, but not available!");
	}

	// application information
	AppInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	AppInfo.pApplicationName = (const char*) "Hello Triangle";
	AppInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	AppInfo.pEngineName = "No Engine";
	AppInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	AppInfo.apiVersion = VK_API_VERSION_1_0;

	// retrieve the required extensions to interface with window system
	glfwExtensions = this->GetRequiredExtensions(gbEnableValidationLayers);

	// instance creation information
	CreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	CreateInfo.pApplicationInfo = &AppInfo;

	// extensions
	CreateInfo.enabledExtensionCount = static_cast<uint32_t>(glfwExtensions.size());
	CreateInfo.ppEnabledExtensionNames = glfwExtensions.data();

	// validation layers
	if (gbEnableValidationLayers)
	{
		// TODO: this is assuming these are in use - this should really be configurable
		CreateInfo.enabledLayerCount = static_cast<uint32_t>(ValidationLayers.size());
		CreateInfo.ppEnabledLayerNames = ValidationLayers.data();
	}
	else
	{
		CreateInfo.enabledLayerCount = 0;
	}
		

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
	DestroyDebugReportCallbackEXT(this->VInstance, this->VDebugReportCallback, nullptr);
	glfwDestroyWindow(this->GlfwWindow);
	vkDestroyInstance(this->VInstance, nullptr);
	glfwTerminate();
}

// Note: C++ 11 std::vector<T> has std::move<T> semantics
std::vector<const char*> VulkanWindowApplication::GetRequiredExtensions(bool bEnableValidationLayers)
{
	std::vector<const char*> Extensions;
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions = nullptr;

	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	for (uint32_t i = 0; i < glfwExtensionCount; i++)
	{
		Extensions.push_back(glfwExtensions[i]);
	}

	// if we have layers enabled add the "VK_EXT_debug_report" extension
	if (bEnableValidationLayers)
	{
		Extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
	}

	return Extensions;
}

// Note: C++ 11 std::vector<T> has std::move<T> semantics
std::vector<VkExtensionProperties> VulkanWindowApplication::GetSupportedExtensions()
{
	uint32_t ExtensionCount = 0;
	std::vector<VkExtensionProperties> SupportedExtensions;
	// get count - jave to reserve space in the vector in order to get it to write correctly
	vkEnumerateInstanceExtensionProperties(nullptr, &ExtensionCount, nullptr);
	// creates enough space for the default state of VkExtensionProperties struct
	SupportedExtensions.resize(ExtensionCount);
	// get count and record in array
	vkEnumerateInstanceExtensionProperties(nullptr, &ExtensionCount, SupportedExtensions.data());
	std::cout << "Enumerated " << SupportedExtensions.size() << " extensions." << std::endl;
	return SupportedExtensions;
}

VKAPI_ATTR VkBool32 VKAPI_CALL VulkanWindowApplication::DebugCallback(VkDebugReportFlagsEXT Flags, VkDebugReportObjectTypeEXT ObjType, uint64_t Obj, size_t Location, int32_t Code, const char * LayerPrefix, const char * Msg, void * UserData)
{
	std::cerr << "validation layer: " << Msg << std::endl;
	return VK_FALSE;
}
