#pragma once
#include <d3d12.h>
#include <DirectXMath.h>
#include <HexEngine/Graphics/DirectX12/Resource.hpp>
#include <HexEngine/Graphics/DirectX12/CommandList.hpp>
#include <HexEngine/World/Camera.hpp>

namespace HexEngine::World
{
	// TODO: Being lazy, this should be moved to a separate file.
	struct Transform
	{
		DirectX::XMFLOAT4X4A world;
		DirectX::XMFLOAT4X4A worldView;
		DirectX::XMFLOAT4X4A worldViewProj;
	};

	class Object
	{
	public:
		Object(Graphics::DirectX12::Device &device, const DirectX::XMFLOAT4X4A &matrix);

		void Update(Graphics::DirectX12::CommandList &cmdList, const Camera &camera);

		[[nodiscard]] const Graphics::DirectX12::Resource &GetWorldBuffer() const { return mWorldBuffer; }

		[[nodiscard]] const DirectX::XMFLOAT4X4A &GetWorldMatrix() const { return mTransform.world; }

		void SetWorldMatrix(const DirectX::XMFLOAT4X4A &matrix) { mTransform.world = matrix; }

	private:
		Graphics::DirectX12::Resource mWorldBuffer;
		Transform mTransform;
	};
}