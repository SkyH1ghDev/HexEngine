#pragma once

// ---------------------- //
//                        //
//    STANDARD LIBRARY    //
//                        //
// ---------------------- //

#include <iostream>
#include <print>
#include <format>
#include <vector>
#include <array>
#include <string>
#include <string_view>
#include <filesystem>
#include <memory>
#include <cstdint>

// ---------------------- //
//                        //
//    LINKED LIBRARIES    //
//                        //
// ---------------------- //

// Windows
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#undef max
#undef min
#undef CreateWindow

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

#include <DirectXMath.h>
#include <DirectXCollision.h>

// Dear ImGui
#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_dx12.h>
#include <ImGui/imgui_impl_sdl3.h>

// ---------------------- //
//                        //
//         SOURCE         //
//                        //
// ---------------------- //

#include <HexEngine/Graphics/DirectX12/BackBuffer.hpp>
#include <HexEngine/Graphics/DirectX12/CommandAllocator.hpp>
#include <HexEngine/Graphics/DirectX12/CommandList.hpp>
#include <HexEngine/Graphics/DirectX12/CommandQueue.hpp>
#include <HexEngine/Graphics/DirectX12/DebugLayer.hpp>
#include <HexEngine/Graphics/DirectX12/DescriptorHeap.hpp>
#include <HexEngine/Graphics/DirectX12/Device.hpp>
#include <HexEngine/Graphics/DirectX12/Fence.hpp>
#include <HexEngine/Graphics/DirectX12/IndexBuffer.hpp>
#include <HexEngine/Graphics/DirectX12/Resource.hpp>
#include <HexEngine/Graphics/DirectX12/SwapChain.hpp>
#include <HexEngine/Graphics/DirectX12/SwapChainManager.hpp>
#include <HexEngine/Graphics/DirectX12/VertexBuffer.hpp>
#include <HexEngine/Graphics/DirectX12/DX12DeviceCapabilities.hpp>
#include <HexEngine/Graphics/DirectX12/DX12Renderer.hpp>
#include <HexEngine/Graphics/DirectX12/DX12RendererSetup.hpp>
#include <HexEngine/Graphics/UI/ImGuiHeapAllocator.hpp>
#include <HexEngine/Utils/DirectXUtils.hpp>
#include <HexEngine/Utils/StringUtils.hpp>
#include <HexEngine/Time/Clock.hpp>