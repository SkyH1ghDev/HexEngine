#pragma once
#include <d3d12.h>
#include <DirectXMath.h>
#include <HexEngine/Graphics/DirectX12/Resource.hpp>
#include <HexEngine/Graphics/DirectX12/CommandList.hpp>
#include <HexEngine/World/Camera.hpp>

namespace HexEngine::World
{
	// HACK: Being lazy, this should be moved to a separate file.
	struct Transform
	{
		DirectX::XMFLOAT4X4A world;
		DirectX::XMFLOAT4X4A worldView;
		DirectX::XMFLOAT4X4A worldViewProj;
	};

	class Object
	{
	public:
		Object(Graphics::DirectX12::Device &device, const DirectX::XMFLOAT4X4A &matrix, const std::string &name, void *meshPtr);

		void Update(Graphics::DirectX12::CommandList &cmdList, const Camera &camera);

		[[nodiscard]] const Graphics::DirectX12::Resource	&GetWorldBuffer() const	{ return mWorldBuffer; }
		[[nodiscard]] const DirectX::XMFLOAT4X4A			&GetWorldMatrix() const	{ return mTransform.world; }
		[[nodiscard]] const std::string						&GetName() const		{ return mName; }
		[[nodiscard]] void									*GetMeshPtr() const		{ return mMeshPtr; }
		
		void SetWorldMatrix(const DirectX::XMFLOAT4X4A &matrix)	{ mTransform.world = matrix; }

	private:
		std::string mName = "Object";
		Graphics::DirectX12::Resource mWorldBuffer;
		Graphics::DirectX12::Resource mWorldUploadBuffer;
		Transform mTransform;
		void *mMeshPtr = nullptr;
	};
}