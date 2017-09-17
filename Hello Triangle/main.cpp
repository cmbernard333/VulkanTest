#include "VulkanWindowApplication.hpp"

#include <iostream>
#include <stdexcept>

int main(int argc, char** argv)
{
	VulkanWindowApplication App;

	try {
		App.Run();
	}
	catch (const std::runtime_error& e) {
		// One example of an error is finding out that a certain required extension is not supported
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}