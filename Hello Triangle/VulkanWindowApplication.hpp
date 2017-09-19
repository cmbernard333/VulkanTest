#pragma once
#define GLFW_INCLUDE_VULKAN // force GLFW to include vulkan header
#include <GLFW/glfw3.h>
#include <vector>
#include <array>
#include <cstdlib>

constexpr int WIDTH = 800;
constexpr int HEIGHT = 600;
#ifndef NDEBUG
constexpr bool gbEnableValidationLayers = true;
#else
constexpr bool gbEnableValidationLayers = false;
#endif

// static array of the validation layers to use if support is enabled
constexpr std::array<const char*,1> ValidationLayers {
	"VK_LAYER_LUNARG_standard_validation"
};

constexpr char VK_CREATE_DEBUG_REPORT_CALLBACK_EXT_FNAME[] = "vkCreateDebugReportCallbackEXT";
constexpr char VK_DESTROY_DEBUG_REPORT_CALLBACK_EXT_FNAME[] = "vkDestroyDebugReportCallbackEXT";

// TODO: move proxy functions to a separate header
// proxy function to load the pointer address of "vkCreateDebugReportCallbackEXT" because
// it is an extension function that requires loading
VkResult CreateDebugReportCallbackEXT(VkInstance Instance, const VkDebugReportCallbackCreateInfoEXT* pCreateInfo,
	const VkAllocationCallbacks* pAllocator, VkDebugReportCallbackEXT* pCallback);

// proxy function to destroy the pointer created as a part of "CreateDebugReportCallbackEXT"
void DestroyDebugReportCallbackEXT(VkInstance Instance, VkDebugReportCallbackEXT Callback, const VkAllocationCallbacks* pAllocator);

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
	/**
	 * Checks if all the requested layers are available
	 */
	bool CheckValidationLayerSupport(const char* const* LayerNames = ValidationLayers.data(), std::size_t LayerNamesCount = ValidationLayers.size());
	/*
	* Setups the debug callback(s) if validation layers are enabled for Vulkan
	*/
	void SetupDebugCallback();
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

	/*
	 * Retrieves the required extensions for glfw to interact with this platform
	 * @Param bEnableValidationLayers - if true the validation layer extension is added
	 * @Return - vector containing the required extensions for this platform
	 */
	std::vector<const char*> GetRequiredExtensions(bool bEnableValidationLayers);

	/*
	 * Retrieves all the supported vulkan extensions for this platform
	 * @Return vector of all the supported extensions
	 */
	std::vector<VkExtensionProperties> GetSupportedExtensions();


	/*
	* Static Debug Callback for reporting extension names using the validation layer API
	* @Param Flags = the type of callback message, bitmask
	* @Param ObjType = specifies the type of object that is the subject of the message
	* @Param obj = the actual vulkan object
	* @Param location = 
	* @Param code = 
	* @Param LayerPrefix = 
	* @Param Msg = the debug message itself
	* @Param UserData = user data passed to the callback
	* @Return - a boolean that indicates if the Vulkan call that triggered the validation layer should be aborted
	*/
	static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
		VkDebugReportFlagsEXT Flags,
		VkDebugReportObjectTypeEXT ObjType,
		uint64_t Obj,
		size_t Location,
		int32_t Code,
		const char* LayerPrefix,
		const char* Msg,
		void* UserData);

	// Window pointer
	GLFWwindow* GlfwWindow;
	// Vulkan Instance
	VkInstance VInstance;
	// the callback handle
	VkDebugReportCallbackEXT VDebugReportCallback;
};