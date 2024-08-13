#include "CameraBase.h"
using namespace DirectX;

CameraBase::CameraBase():mFovY(DirectX::XM_PIDIV2), mAspect(gD3D->AspectRatio()), mNearZ(0.2f), mFarZ(1000.0f)
{
}


DirectX::XMMATRIX CameraBase::GetViewXM() const
{
	return mTransform.GetWorldToLocalMatrixXM();
}

DirectX::XMMATRIX CameraBase::GetProjXM(bool isReversed) const
{
	if (isReversed)
	{
		return XMMatrixPerspectiveFovLH(mFovY, mAspect, mFarZ, mNearZ);
	}
	else{
		return XMMatrixPerspectiveFovLH(mFovY, mAspect, mNearZ, mFarZ);
	}
}


DirectX::XMFLOAT4X4 CameraBase::GetViewXMF(bool isTranspose) const
{
	DirectX::XMMATRIX mat = GetViewXM();
	if(isTranspose)
		mat = DirectX::XMMatrixTranspose(mat);
	DirectX::XMFLOAT4X4 fMat;
	DirectX::XMStoreFloat4x4(&fMat, mat);
	return fMat;
}

DirectX::XMFLOAT4X4 CameraBase::GetProjXMF(bool isTranspose) const
{
	DirectX::XMMATRIX mat = GetProjXM();
	if (isTranspose)
		mat = DirectX::XMMatrixTranspose(mat);
	DirectX::XMFLOAT4X4 fMat;
	DirectX::XMStoreFloat4x4(&fMat, mat);
	return fMat;
}

void CameraBase::ResetCamera()
{
	mTransform.SetPosition(0, 0, -5.0f);
	mTransform.SetRotation(0, 0, 0);
}

