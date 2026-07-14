#include "EngineLoop.hpp"
#include "EngineSetup.hpp"
#include "EngineCore.hpp"

#include <HexEngine/Input/Input.hpp>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_vulkan.h>
#include <vulkan/vulkan.hpp>
#include <print>
#include <iostream>

void EngineCore::Run()
{
    Input::Initialize();
    Input::BindKey(SDL_SCANCODE_ESCAPE, PostQuitEvent_Callback);

    SDLWindow window(EngineSetup::InitializeWindow());

    VkApplicationInfo appInfo
    {
        .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pNext = nullptr,
        .pApplicationName = "Hello Triangle!",
        .applicationVersion = VK_MAKE_VERSION(0, 0, 1),
        .pEngineName = "HexEngine",
        .engineVersion = VK_MAKE_VERSION(0, 0, 1),
        .apiVersion = VK_API_VERSION_1_4
    };

    std::uint32_t numExtensions = 0;
    char const * const * vulkanExtensions = SDL_Vulkan_GetInstanceExtensions(&numExtensions);

    VkInstanceCreateInfo createInfo
    {
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .pApplicationInfo = &appInfo,
        .enabledLayerCount = 0,
        .ppEnabledLayerNames = nullptr,
        .enabledExtensionCount = numExtensions,
        .ppEnabledExtensionNames = vulkanExtensions
    };

    VkInstance instance {};
    if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create instance!");
    }

    std::uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> extensions(extensionCount);

    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

    std::print("available extensions:\n");

    for (const auto& [extensionName, specVersion] : extensions)
    {
        std::cout << "\t" << extensionName << "\n";
    }

    vkDestroyInstance(instance, nullptr);

    while (true) {} // Loop
    Quit();
}

void EngineCore::Quit()
{
    SDL_Quit();
}

void EngineCore::PostQuitEvent_Callback(bool keyState)
{
    if (!keyState)
    {
        return;
    }
    
    SDL_Event event;
    SDL_zero(event);
    event.type = SDL_EVENT_QUIT;
    event.user.code = 0;
    event.user.data1 = nullptr;
    event.user.data2 = nullptr;
    SDL_PushEvent(&event);
}



