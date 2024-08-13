#pragma once
#include <DirectXMath.h>
#include "Transform.h"

namespace CameraSet
{
	enum CameraMode
	{
		CAM_NONE,
		CAM_FIRST_PERSON,
		CAM_THIRD_FREE,
		CAM_THIRD_TRACK,
	};
}

class CameraBase
{
protected:
	
	// Frustum Param
	float mNearZ = 0.1f;
	float mFarZ = 0.0f;
	float mAspect = 0.0f;
	float mFovY = 0.0f;//Default:90°

public:
	CameraSet::CameraMode mMode = CameraSet::CAM_NONE;

	Transform mTransform = {};

	CameraBase();
	virtual ~CameraBase() = default;
	virtual void Update(float dt) = 0;

	/// @brief 位置取得
	const DirectX::XMFLOAT3 GetPos() { return mTransform.GetPosition(); };
	/// @brief 位置設定
	void SetPos(const DirectX::XMFLOAT3& pos) { mTransform.SetPosition(pos); };

	/// @brief 上方向取得
	const DirectX::XMFLOAT3 GetUpDir() { return mTransform.GetUpAxis(); };

	DirectX::XMMATRIX GetViewXM() const;
	DirectX::XMMATRIX GetProjXM(bool isReversed = false) const;

	/// @brief ビュー行列を取得
	/// @param transpose 行列の行と列を交換するか
	/// @return 
	DirectX::XMFLOAT4X4 GetViewXMF(bool isTranspose = true) const;

	/// @brief 投影行列を取得
	/// @param transpose 行列の行と列を交換するか
	/// @return 
	DirectX::XMFLOAT4X4 GetProjXMF(bool isTranspose = true) const;

	/// @brief カメラのパターンを設定
	/// @param _mode 
	void SetCameraMode(CameraSet::CameraMode _mode) { mMode = _mode; }

	void ResetCamera();

};
