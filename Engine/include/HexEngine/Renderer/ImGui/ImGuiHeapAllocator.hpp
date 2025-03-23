#pragma once
#include <winrt/base.h>
#include <directx/d3dx12.h>
#include <ImGui/imgui.h>
#include <HexEngine/Renderer/DirectX/Device.hpp>
#include <HexEngine/Renderer/DirectX/DescriptorHeap.hpp>

class ImGuiHeapAllocator
{
public:
    ImGuiHeapAllocator() = default;
    ImGuiHeapAllocator(const Device& device, const DescriptorHeap& descriptorHeap);
    ~ImGuiHeapAllocator() = default;
    ImGuiHeapAllocator(const ImGuiHeapAllocator& other) = default;
    ImGuiHeapAllocator& operator=(const ImGuiHeapAllocator& other) = default;
    ImGuiHeapAllocator(ImGuiHeapAllocator&& other) noexcept = default;
    ImGuiHeapAllocator& operator=(ImGuiHeapAllocator&& other) noexcept = default;

    void Allocate(D3D12_CPU_DESCRIPTOR_HANDLE* cpuHandle, D3D12_GPU_DESCRIPTOR_HANDLE* gpuHandle);
    void Deallocate(const D3D12_CPU_DESCRIPTOR_HANDLE& cpuHandle, const D3D12_GPU_DESCRIPTOR_HANDLE& gpuHandle);

    winrt::com_ptr<ID3D12DescriptorHeap> GetDescriptorHeap();
    
private:
    winrt::com_ptr<ID3D12DescriptorHeap>    m_descriptorHeap = nullptr;
    D3D12_CPU_DESCRIPTOR_HANDLE             m_heapStartCPU;
    D3D12_GPU_DESCRIPTOR_HANDLE             m_heapStartGPU;
    UINT                                    m_heapHandleIncrement;
    ImVector<std::uint64_t>                 m_freeIndices = {};

};

inline winrt::com_ptr<ID3D12DescriptorHeap> ImGuiHeapAllocator::GetDescriptorHeap()
{
    return m_descriptorHeap;
}