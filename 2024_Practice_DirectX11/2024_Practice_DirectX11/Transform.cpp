#include "Transform.h"
using namespace DirectX::SimpleMath;
using namespace DirectX;
DirectX::XMFLOAT4X4 Transform::GetMatrix()
{
	GenerateWorld();
	return mMatrix;
}

void Transform::Update()
{
	
}

void Transform::GenerateWorld()
{/*
	Matrix rot = Matrix::CreateFromYawPitchRoll(mRotation.y, mRotation.x, mRotation.z);
	Matrix scale = Matrix::CreateScale(mScale.x, mScale.y, mScale.z);
	Matrix pos = Matrix::CreateTranslation(mPos.x, mPos.y, mPos.z);

	Matrix worldMatrix = scale * rot * pos;*/
	XMMATRIX matrixMove = XMMatrixTranslation(mPos.x, mPos.y, mPos.z);
	//ägëÂèkè¨çsóÒ
	XMMATRIX matrixScale = XMMatrixScaling(mScale.x, mScale.y, mScale.z);

	//âÒì]çsóÒ
	XMMATRIX matrixRotateX = XMMatrixRotationX(XMConvertToRadians(mRotation.x));
	XMMATRIX matrixRotateY = XMMatrixRotationY(XMConvertToRadians(mRotation.y));
	XMMATRIX matrixRotateZ = XMMatrixRotationZ(XMConvertToRadians(mRotation.z));
	XMMATRIX matrixRotate = matrixRotateX * matrixRotateY * matrixRotateZ;
	XMMATRIX matrixWorld = matrixScale * matrixRotate * matrixMove;

	//DirectX::XMStoreFloat4x4(&mMatrix, matrixWorld);
	XMStoreFloat4x4(&mMatrix, matrixWorld);
}
