#include "EngineLoop.hpp"
#include "EngineSetup.hpp"
#include "EngineCore.hpp"

#include <HexEngine/Input/Input.hpp>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_vulkan.h>
#include <vulkan/vulkan_raii.hpp>
#include <print>
#include <iostream>

void EngineCore::Run()
{
    Input::Initialize();
    Input::BindKey(SDL_SCANCODE_ESCAPE, PostQuitEvent_Callback);

    SDLWindow window(EngineSetup::InitializeWindow());

    const vk::raii::Context context {};
    const std::uint32_t vkApiVersion { context.enumerateInstanceVersion() };

    const vk::ApplicationInfo appInfo {
        "Editor",
        VK_MAKE_API_VERSION(0, 0, 0, 0),
        "HexEngine",
        VK_MAKE_API_VERSION(0, 0, 0, 1),
        vkApiVersion,
       {}
    };

    std::uint32_t numExtensions = 0;
    char const * const * sdlVulkanExt = SDL_Vulkan_GetInstanceExtensions(&numExtensions);

    const vk::InstanceCreateInfo createInfo {
        {},
        &appInfo,
        {},
        {},
        {},
        sdlVulkanExt,
        {}
    };

    const vk::raii::Instance instance { context, createInfo };

    const std::vector<vk::PhysicalDeviceGroupProperties> physicalDeviceGroupProperties { instance.enumeratePhysicalDeviceGroups() };

    for (vk::PhysicalDeviceGroupProperties a : physicalDeviceGroupProperties)
    {
        std::cout << a.physicalDeviceCount << "\n";
    }

    const vk::raii::PhysicalDevices physicalDevices { instance };

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



