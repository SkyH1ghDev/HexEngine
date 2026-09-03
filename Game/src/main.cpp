//-- Includes --//
#include <HexEngine/Engine/EngineCore.hpp>
#include <print>
#include <vulkan/vulkan_raii.hpp>

#ifdef _WIN32
#define _CRTDBG_MAP_ALLOC

#include <crtdbg.h>
#endif


int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
#ifdef _WIN32
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif


    vk::Instance instance{};

    vk::PhysicalDevice device{};

    //EngineCore::Run();
    std::println("Hello 1");
    std::print("Hello");
    return 0;
}
