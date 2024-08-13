#include "Camera.h"

Camera::Camera():mFovY(DirectX::XMConvertToRadians(60.0f)), mAspect(gD3D->AspectRatio()), mNearZ(0.2f), mFarZ(1000.0f)
{
}

Camera::~Camera()
{
}

DirectX::XMFLOAT4X4 Camera::GetView(bool transpose) const
{
	DirectX::XMMATRIX mat = DirectX::XMMatrixLookAtLH(
		DirectX::XMLoadFloat3(&mPos), 
		DirectX::XMLoadFloat3(&mFocus),
		DirectX::XMLoadFloat3(&mUpDir)
	);

	if (transpose)
		mat = DirectX::XMMatrixTranspose(mat);
	DirectX::XMFLOAT4X4 fMat;
	DirectX::XMStoreFloat4x4(&fMat, mat);
	return fMat;
}

DirectX::XMFLOAT4X4 Camera::GetProj(bool transpose) const
{
	DirectX::XMMATRIX mat = DirectX::XMMatrixPerspectiveFovLH(mFovY, mAspect, mNearZ, mFarZ);
	if (transpose)
		mat = DirectX::XMMatrixTranspose(mat);
	DirectX::XMFLOAT4X4 fMat;
	DirectX::XMStoreFloat4x4(&fMat, mat);
	return fMat;
}

DirectX::XMFLOAT3 Camera::GetPos() const
{
	return mPos;
}

DirectX::XMFLOAT3 Camera::GeFocus() const
{
	return mFocus;
}

DirectX::XMFLOAT3 Camera::GetUp() const
{
	return mUpDir;
}

void Camera::SetPos(DirectX::XMFLOAT3 pos)
{
	mPos = pos;
}

void Camera::SetFocus(DirectX::XMFLOAT3 look)
{
	mFocus = look;
}

void Camera::SetUp(DirectX::XMFLOAT3 up)
{
	mUpDir = up;
}

