﻿#include "CameraBase.h"
#include "GampApp.h"
using namespace DirectX;
using namespace DirectX::SimpleMath;

CameraBase::CameraBase():mFovY(DirectX::XM_PI/3), mAspect(GameApp::Get()->AspectRatio()), mNearZ(0.1f), mFarZ(1000.0f)
{
}

const DirectX::XMMATRIX CameraBase::GetViewXM() noexcept
{
	return mTransform.GetWorldToLocalMatrixXM();
}

const DirectX::XMMATRIX CameraBase::GetProjXM(bool isReversed) noexcept
{
	if (isReversed)
	{
		return XMMatrixPerspectiveFovLH(mFovY, mAspect, mFarZ, mNearZ);
	
	}
	else{
		return XMMatrixPerspectiveFovLH(mFovY, mAspect, mNearZ, mFarZ);
	}
}

const DirectX::XMFLOAT4X4 CameraBase::GetViewXMF(bool isTranspose) noexcept
{
	DirectX::XMMATRIX mat = GetViewXM();
	if(isTranspose)
		mat = DirectX::XMMatrixTranspose(mat);
	DirectX::XMFLOAT4X4 fMat;
	DirectX::XMStoreFloat4x4(&fMat, mat);
	return fMat;
}

const DirectX::XMFLOAT4X4 CameraBase::GetProjXMF(bool isTranspose) noexcept
{
	DirectX::XMMATRIX mat = GetProjXM();
	if (isTranspose)
		mat = DirectX::XMMatrixTranspose(mat);
	DirectX::XMFLOAT4X4 fMat;
	DirectX::XMStoreFloat4x4(&fMat, mat);
	return fMat;
}


void CameraBase::ResetCamera() noexcept
{
	mTransform.SetPosition(defaultPos);
	mTransform.SetRotationInDegree(0, 0, 0);
}

DirectX::XMVECTOR CameraBase::MousePosToWorld(POINT mousePos) noexcept
{
	DirectX::XMFLOAT3 worldPos={};
	ScreenToClient(gD3D->MainWnd(), &mousePos); // 转换为窗口内坐标
	XMVECTOR screenPos = XMVectorSet((float)mousePos.x, (float)mousePos.y, 0.0f, 1.0f);

	XMVECTOR worldPosVec = XMVector3Unproject(
		screenPos,
		0.0f, 0.0f,    // 视口的X和Y
		static_cast<float>(gD3D->GetWinWidth()), static_cast<float>(gD3D->GetWinHeight()), // 视口的宽度和高度
		mNearZ, mFarZ,    // 最小深度和最大深度
		this->GetProjXM(),   // 投影矩阵
		this->GetViewXM(),   // 视图矩阵
		XMMatrixIdentity()   // 世界矩阵
	);

	return worldPosVec;

}

DirectX::XMVECTOR CameraBase::ScreenPointToRay(POINT mousePos) noexcept
{
	Vector3 cameraPos = mTransform.GetPosition();
	Vector3 mouseWorldPos = MousePosToWorld(mousePos);
	XMVECTOR rayDir = XMVector3Normalize(mouseWorldPos - cameraPos);
	return rayDir;
}

