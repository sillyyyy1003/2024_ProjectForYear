#pragma once
#include "Transform.h"

class Camera
{
protected:
	Transform mTransform = {};

	//viewing frustum
	float mNearZ = 0.0f;
	float mFarZ = 0.0f;
	float mAspect = 0.0f;
	float mFovY = 0.0f;

	//parameter in view/projection
	DirectX::XMFLOAT3 mPos = { 0.0f, 0.0f, -10.0f };
	DirectX::XMFLOAT3 mFocus = { 0.0f, 0.0f, 0.0f };
	DirectX::XMFLOAT3 mUpDir = {0.f,1.0f,0.0f};
public:
	Camera();
	virtual ~Camera();//pure virtual functions
	virtual void Update() = 0;//pure virtual functions

	/// @brief Get View Matrix
	/// @param transpose 行列の行と列を交換するか
	/// @return 
	DirectX::XMFLOAT4X4 GetView(bool transpose = true) const;

	/// @brief Get Projection Matrix
	/// @param transpose 行列の行と列を交換するか
	/// @return 
	DirectX::XMFLOAT4X4 GetProj(bool transpose = true) const;

	DirectX::XMFLOAT3 GetPos() const;
	DirectX::XMFLOAT3 GeFocus() const;
	DirectX::XMFLOAT3 GetUp() const;

	void SetPos(DirectX::XMFLOAT3 pos);
	void SetFocus(DirectX::XMFLOAT3 look);
	void SetUp(DirectX::XMFLOAT3 up);
};

