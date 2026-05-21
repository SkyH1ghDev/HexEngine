#include "Object.hpp"

using namespace DirectX;
using namespace DirectXUtils;
using namespace HexEngine::Graphics::DirectX12;

HexEngine::World::Object::Object(Device &device, const DirectX::XMFLOAT4X4A &matrix)
	: mTransform{matrix, {}, {}}
{
	// Create world buffer
	auto worldBufferDesc = CD3DX12_RESOURCE_DESC::Buffer(sizeof(Transform));

	auto defaultHeap = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

	auto worldBufferCOM = mWorldBuffer.GetCOM();

	ThrowIfFailed(device.GetRaw()->CreateCommittedResource(
		&defaultHeap, 
		D3D12_HEAP_FLAG_NONE, 
		&worldBufferDesc, 
		D3D12_RESOURCE_STATE_COPY_DEST, 
		nullptr, 
		IID_PPV_ARGS(&worldBufferCOM)
	));
}

void HexEngine::World::Object::Update(CommandList &cmdList, const Camera &camera)
{
	// Update transform by calculating worldView and worldViewProj matrices
	XMMATRIX world = XMLoadFloat4x4(&mTransform.world);
	XMMATRIX view = XMLoadFloat4x4(&camera.GetView());
	XMMATRIX projection = XMLoadFloat4x4(&camera.GetProjection());

	XMMATRIX worldView = world * view;
	XMMATRIX worldViewProj = worldView * projection;

	XMStoreFloat4x4(&mTransform.worldView, worldView);
	XMStoreFloat4x4(&mTransform.worldViewProj, worldViewProj);

	// Upload new transform to GPU
	cmdList.GetRaw()->CopyBufferRegion(
		mWorldBuffer.GetRaw(), 0, 
		nullptr, 0, 
		sizeof(Transform)
	);
}
