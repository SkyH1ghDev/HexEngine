#include "Object.hpp"

using namespace DirectX;
using namespace DirectXUtils;
using namespace HexEngine::Graphics::DirectX12;

HexEngine::World::Object::Object(Device &device, const DirectX::XMFLOAT4X4A &matrix, const std::string &name, void *meshPtr)
	: mTransform{ matrix, {}, {} }, mName(name), mMeshPtr(meshPtr)
{
	// Create world buffer
	auto worldBufferDesc = CD3DX12_RESOURCE_DESC::Buffer(sizeof(Transform));

	auto defaultHeap = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	auto uploadHeap = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);

	auto &worldBufferCOM = mWorldBuffer.GetCOM();
	auto &worldUploadBufferCOM = mWorldUploadBuffer.GetCOM();

    // Default Heap
    ThrowIfFailed(device.GetRaw()->CreateCommittedResource(
        &defaultHeap,
        D3D12_HEAP_FLAG_NONE,
        &worldBufferDesc,
        D3D12_RESOURCE_STATE_COMMON, // IMPORTANT: start as copy destination
        nullptr,
        IID_PPV_ARGS(&worldBufferCOM)
    ));

	// Upload Heap
    ThrowIfFailed(device.GetRaw()->CreateCommittedResource(
        &uploadHeap,
        D3D12_HEAP_FLAG_NONE,
        &worldBufferDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&worldUploadBufferCOM)
    ));
}

void HexEngine::World::Object::Update([[maybe_unused]] CommandList &cmdList, const Camera &camera)
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
    {
        void *mapped = nullptr;
        CD3DX12_RANGE readRange(0, 0);

        mWorldUploadBuffer.GetCOM()->Map(0, &readRange, &mapped);
        memcpy(mapped, &mTransform, sizeof(Transform));
        mWorldUploadBuffer.GetCOM()->Unmap(0, nullptr);

        auto barrierToCopyDest = CD3DX12_RESOURCE_BARRIER::Transition(
            mWorldBuffer.GetRaw(),
            D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER,
            D3D12_RESOURCE_STATE_COPY_DEST
        );

        cmdList.GetCOM()->ResourceBarrier(1, &barrierToCopyDest);

        cmdList->CopyBufferRegion(
            mWorldBuffer.GetRaw(),
            0,
            mWorldUploadBuffer.GetRaw(),
            0,
            sizeof(Transform)
        );

        auto barrierToShader = CD3DX12_RESOURCE_BARRIER::Transition(
            mWorldBuffer.GetRaw(),
            D3D12_RESOURCE_STATE_COPY_DEST,
            D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER
        );

        cmdList.GetCOM()->ResourceBarrier(1, &barrierToShader);
    }
}
