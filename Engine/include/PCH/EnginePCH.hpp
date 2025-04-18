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

// COM-Objects
#include <winrt/base.h>

// DirectX
#include <directx/d3dx12.h>
#include <dxgi.h>
#include <dxgi1_2.h>
#include <dxgi1_3.h>
#include <dxgi1_4.h>
#include <dxgi1_5.h>
#include <dxgi1_6.h>

// Dear ImGui
#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_dx12.h>
#include <ImGui/imgui_impl_sdl3.h>

// ---------------------- //
//                        //
//         SOURCE         //
//                        //
// ---------------------- //

#include <HexEngine/Renderer/DirectX/BackBuffer.hpp>
#include <HexEngine/Renderer/DirectX/CommandAllocator.hpp>
#include <HexEngine/Renderer/DirectX/CommandList.hpp>
#include <HexEngine/Renderer/DirectX/CommandQueue.hpp>
#include <HexEngine/Renderer/DirectX/DebugLayer.hpp>
#include <HexEngine/Renderer/DirectX/DescriptorHeap.hpp>
#include <HexEngine/Renderer/DirectX/Device.hpp>
#include <HexEngine/Renderer/DirectX/DirectXUtils.hpp>
#include <HexEngine/Renderer/DirectX/Fence.hpp>
#include <HexEngine/Renderer/DirectX/IndexBuffer.hpp>
#include <HexEngine/Renderer/DirectX/Resource.hpp>
#include <HexEngine/Renderer/DirectX/SwapChain.hpp>
#include <HexEngine/Renderer/DirectX/SwapChainManager.hpp>
#include <HexEngine/Renderer/DirectX/VertexBuffer.hpp>
#include <HexEngine/Renderer/ImGui/ImGuiHeapAllocator.hpp>
#include <HexEngine/Clock/Clock.hpp>