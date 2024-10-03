#pragma once
#include <DirectXMath.h>
#include "SceneBase.h"
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
};

class CameraBase :public Component
{
protected:
	
	// Frustum Param
	float mNearZ = 0.1f;
	float mFarZ = 0.0f;
	float mAspect = 0.0f;
	float mFovY = 0.0f;//Default:90°

	const DirectX::XMFLOAT3 defaultPos={0,10.f,-10.f};
	const DirectX::XMFLOAT3 defaultTarget = { 0,0,0 };
	const DirectX::XMFLOAT3 defaultUpAxis = { 0,1,0 };

public:

	CameraSet::CameraMode mMode = CameraSet::CAM_NONE;
	Transform mTransform = {};

	CameraBase();
	virtual ~CameraBase() = default;
	virtual void Update(float dt) = 0;

	/// @brief 位置取得
	DirectX::XMFLOAT3 GetPos() const noexcept { return mTransform.GetPosition(); };
	/// @brief 位置設定
	void SetPos(const DirectX::XMFLOAT3& pos) noexcept { mTransform.SetPosition(pos); };

	/// @brief 上方向取得
	const DirectX::XMFLOAT3 GetUpDir()noexcept { return mTransform.GetUpAxis(); };

	const DirectX::XMMATRIX GetViewXM()noexcept;
	const DirectX::XMMATRIX GetProjXM(bool isReversed = false)noexcept;

	/// @brief ビュー行列を取得
	/// @param isTranspose 行列の行と列を交換するか
	/// @return 
	const  DirectX::XMFLOAT4X4 GetViewXMF(bool isTranspose = true)noexcept;

	/// @brief 投影行列を取得
	/// @param isTranspose 行列の行と列を交換するか
	/// @return 
	const DirectX::XMFLOAT4X4 GetProjXMF(bool isTranspose = true) noexcept;

	/// @brief カメラのパターンを設定
	/// @param _mode 
	void SetCameraMode(CameraSet::CameraMode _mode) noexcept { mMode = _mode; }

	/// @brief カメラを指定位置にリセット
	void ResetCamera() noexcept;


	/// @brief マウス座標をワールド座標に変換する
	/// @param mousePos マウス座標
	/// @return ワールド座標
	DirectX::XMVECTOR MousePosToWorld(POINT mousePos) noexcept;

	/// @brief カメラからマウス方向の射線をお求める
	/// @param mousePos マウス座標
	/// @return 射線ベクトル
	DirectX::XMVECTOR ScreenPointToRay(POINT mousePos) noexcept;

	const DirectX::XMFLOAT3& GetDefaultUpAxis() noexcept { return defaultUpAxis; };

};
