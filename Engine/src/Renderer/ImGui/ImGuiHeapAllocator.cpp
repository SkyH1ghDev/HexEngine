#include "ImGuiHeapAllocator.hpp"

ImGuiHeapAllocator::ImGuiHeapAllocator(const Device& device, const DescriptorHeap& descriptorHeap)
{
    m_descriptorHeap = descriptorHeap.GetCOM();
    
    D3D12_DESCRIPTOR_HEAP_DESC desc = m_descriptorHeap->GetDesc();
    m_heapStartCPU = m_descriptorHeap->GetCPUDescriptorHandleForHeapStart();
    m_heapStartGPU = m_descriptorHeap->GetGPUDescriptorHandleForHeapStart();
    m_heapHandleIncrement = device.GetCOM()->GetDescriptorHandleIncrementSize(desc.Type);
    m_freeIndices.reserve(static_cast<int>(desc.NumDescriptors));

    for (std::uint32_t i = desc.NumDescriptors; i > 0; i--)
    {
        m_freeIndices.push_back(i - 1);
    }
}

void ImGuiHeapAllocator::Allocate(D3D12_CPU_DESCRIPTOR_HANDLE* cpuHandle, D3D12_GPU_DESCRIPTOR_HANDLE* gpuHandle)
{
    IM_ASSERT(!m_freeIndices.empty());
    std::uint64_t index = m_freeIndices.back();
    m_freeIndices.pop_back();
    cpuHandle->ptr = m_heapStartCPU.ptr + (index * m_heapHandleIncrement);
    gpuHandle->ptr = m_heapStartGPU.ptr + (index * m_heapHandleIncrement);
}

void ImGuiHeapAllocator::Deallocate(const D3D12_CPU_DESCRIPTOR_HANDLE& cpuHandle, const D3D12_GPU_DESCRIPTOR_HANDLE& gpuHandle)
{
    std::uint64_t cpuIndex = (cpuHandle.ptr - m_heapStartCPU.ptr) / m_heapHandleIncrement;
    std::uint64_t gpuIndex = (gpuHandle.ptr - m_heapStartGPU.ptr) / m_heapHandleIncrement;

    IM_ASSERT(cpuIndex == gpuIndex);
    m_freeIndices.push_back(cpuIndex);
}

