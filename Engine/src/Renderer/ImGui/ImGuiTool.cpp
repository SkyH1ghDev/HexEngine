#include "ImGuiTool.hpp"

#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_sdl3.h>
#include <ImGui/imgui_impl_dx12.h>
#include <SDL3/SDL_mouse.h>
#include <HexEngine/Renderer/RendererSetup.hpp>


ImGuiHeapAllocator ImGuiTool::m_descriptorHeapAllocator = {};
std::array<std::uint8_t, 2> ImGuiTool::m_mouseButtons = { 0, 0 };

void ImGuiTool::Initialize(const SDLWindow& window, const Device& device, const CommandQueue& commandQueue, const DescriptorHeap& srvDescriptorHeap, DXGI_FORMAT backBufferFormat, std::uint64_t maxFrameIndex)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    io.ConfigViewportsNoAutoMerge = true;
	io.ConfigViewportsNoTaskBarIcon = true;

    ImGui::StyleColorsDark();

    ImGui_ImplSDL3_InitForD3D(window.GetSDLWindow());

    m_descriptorHeapAllocator = ImGuiHeapAllocator(device, srvDescriptorHeap);
	winrt::com_ptr<ID3D12DescriptorHeap> descriptorHeap = m_descriptorHeapAllocator.GetDescriptorHeap();
    
    ImGui_ImplDX12_InitInfo initInfo = {};
    initInfo.Device = device.GetRaw();
    initInfo.CommandQueue = commandQueue.GetCOM().get();
    initInfo.NumFramesInFlight = static_cast<std::int32_t>(maxFrameIndex);
    initInfo.RTVFormat = backBufferFormat;
    initInfo.DSVFormat = DXGI_FORMAT_UNKNOWN;
    initInfo.SrvDescriptorHeap = descriptorHeap.get();
    initInfo.SrvDescriptorAllocFn = [](ImGui_ImplDX12_InitInfo*, D3D12_CPU_DESCRIPTOR_HANDLE* out_cpu_handle, D3D12_GPU_DESCRIPTOR_HANDLE* out_gpu_handle) { return m_descriptorHeapAllocator.Allocate(out_cpu_handle, out_gpu_handle); };
    initInfo.SrvDescriptorFreeFn = [](ImGui_ImplDX12_InitInfo*, D3D12_CPU_DESCRIPTOR_HANDLE cpu_handle, D3D12_GPU_DESCRIPTOR_HANDLE gpu_handle)            { return m_descriptorHeapAllocator.Deallocate(cpu_handle, gpu_handle); };
    ImGui_ImplDX12_Init(&initInfo);

}

void ImGuiTool::Start()
{
    ImGui_ImplDX12_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();
}

void ImGuiTool::Run()
{
	// Windows
	ImGui::ShowDemoWindow();
	
	ImGuiTabBarFlags mainTabBarFlags = ImGuiTabBarFlags_None;

	ImGui::BeginTabBar("main", mainTabBarFlags);

	if (ImGui::BeginTabItem("Test"))
	{
		ImGui::EndTabItem();
	}

	ImGui::EndTabBar();
	
	ImGui::Render();
}

void ImGuiTool::RenderDrawData(const CommandList& commandList)
{
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), commandList.GetRaw());	
}

void ImGuiTool::End()
{
	ImGui::UpdatePlatformWindows();
	ImGui::RenderPlatformWindowsDefault();
}

void ImGuiTool::Shutdown()
{
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplSDL3_Shutdown();
	ImGui::DestroyContext();
}