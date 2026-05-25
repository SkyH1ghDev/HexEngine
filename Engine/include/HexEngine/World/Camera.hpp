#pragma once
#include <d3d12.h>
#include <DirectXMath.h>

namespace HexEngine::World
{
	struct CameraParams
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT3 forward;
		DirectX::XMFLOAT3 up;
		float fovY;
		float aspectRatio;
		float nearZ;
		float farZ;
	};

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
		[[nodiscard]] const CameraParams &GetCameraParams() const		{ return mCamParams;	}

		void SetView(DirectX::XMFLOAT4X4A mat)			{ mView = mat;			}
		void SetProjection(DirectX::XMFLOAT4X4A mat)	{ mProjection = mat;	}
		void SetCamera(DirectX::XMFLOAT3 position,
					   DirectX::XMFLOAT3 forward,
					   DirectX::XMFLOAT3 up,
					   float fovY, float aspectRatio,
					   float nearZ, float farZ);

	private:
		DirectX::XMFLOAT4X4A mView {};
		DirectX::XMFLOAT4X4A mProjection {};
		CameraParams mCamParams{};
	};
}