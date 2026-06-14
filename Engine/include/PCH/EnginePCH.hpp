#pragma once

// ---------------------- //
//                        //
//    STANDARD LIBRARY    //
//                        //
// ---------------------- //

#include <iostream>
#include <cstdint>

// ---------------------- //
//                        //
//    LINKED LIBRARIES    //
//                        //
// ---------------------- //

// Vulkan
#include <vulkan/vulkan.hpp>

// Dear ImGui
#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_vulkan.h>
#include <ImGui/imgui_impl_sdl3.h>

// ---------------------- //
//                        //
//         SOURCE         //
//                        //
// ---------------------- //

#include <HexEngine/Renderer/ImGui/ImGuiHeapAllocator.hpp>
#include <HexEngine/Clock/Clock.hpp>