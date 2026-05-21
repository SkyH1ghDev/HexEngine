#pragma once
#include <d3d12.h>
#include <DirectXMath.h>
#include <HexEngine/Graphics/DirectX12/Resource.hpp>

namespace HexEngine::World
{
	class Camera
	{
	public:
		Camera() = default;
		~Camera() = default;
		Camera(const Camera& other) = default;
		Camera& operator=(const Camera& other) = default;
		Camera(Camera&& other) noexcept = default;
		Camera &operator=(Camera &&other) noexcept = default;

		[[nodiscard]] const DirectX::XMFLOAT4X4A &GetView() const		{ return mView;			}
		[[nodiscard]] const DirectX::XMFLOAT4X4A &GetProjection() const { return mProjection;	}

		void SetView(DirectX::XMFLOAT4X4A mat)			{ mView = mat;			}
		void SetProjection(DirectX::XMFLOAT4X4A mat)	{ mProjection = mat;	}

	private:
		DirectX::XMFLOAT4X4A mView;
		DirectX::XMFLOAT4X4A mProjection;
	};
}