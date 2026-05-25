#include "EnginePCH.hpp"
#include "Camera.hpp"

using namespace DirectX;

void HexEngine::World::Camera::SetCamera(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 forward, DirectX::XMFLOAT3 up, float fovY, float aspectRatio, float nearZ, float farZ)
{
	mCamParams = {};
	mCamParams.position = position;
	mCamParams.forward = forward;
	mCamParams.up = up;
	mCamParams.fovY = fovY;
	mCamParams.aspectRatio = aspectRatio;
	mCamParams.nearZ = nearZ;
	mCamParams.farZ = farZ;

	XMVECTOR vPos = XMLoadFloat3(&position);
	XMVECTOR vTarget = XMVectorAdd(vPos, XMVector3Normalize(XMLoadFloat3(&forward)));
	XMVECTOR vUp = XMVector3Normalize(XMLoadFloat3(&up));

	XMMATRIX view = XMMatrixLookAtLH(vPos, vTarget, vUp);
	XMMATRIX projection = XMMatrixPerspectiveFovLH(fovY, aspectRatio, nearZ, farZ);

	XMStoreFloat4x4(&mView, view);
	XMStoreFloat4x4(&mProjection, projection);
}
