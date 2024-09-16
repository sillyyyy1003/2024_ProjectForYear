#pragma once
#include "D3DUtil.h"

///位置、回転、大きさなどの管理
class Transform
{
protected:

	DirectX::XMFLOAT3 mPos = {};
	/// @brief Pitch:x  Yaw:y  Roll:z
	DirectX::XMFLOAT4 mRotation = { 0.0f, 0.0f, 0.0f, 1.0f };
	DirectX::XMFLOAT3 mScale = {1.f,1.f,1.f,};


public:
	Transform() = default;
	Transform(const DirectX::XMFLOAT3& _pos, const DirectX::XMFLOAT4& _rotation, const DirectX::XMFLOAT3& _scale);
	Transform(const Transform&) = default;
	Transform& operator=(const Transform&) = default;
	~Transform() = default;


	/// @brief データを行列に変換
	/// @return 行列を返す
	const DirectX::XMFLOAT4X4 GetMatrixFX4();
	const DirectX::XMMATRIX GetMatrix();

	/// @brief Scale
	DirectX::XMFLOAT3 GetScale() const { return mScale; }
	DirectX::XMVECTOR GetScaleXM() const { return DirectX::XMLoadFloat3(&mScale); }
	
	
	/// @brief Angle in Radian
	/// @return Radian
	DirectX::XMFLOAT3 GetRotation() const;

	/// @brief 回転の四元数を返す
	/// @return 四元数　Pitch:x  Yaw:y  Roll:z
	DirectX::XMFLOAT4 GetRotationQuat() const { return mRotation; }
	DirectX::XMVECTOR GetRotationQuatXM() const { return XMLoadFloat4(&mRotation); }

	/// @brief Position
	/// @return 
	DirectX::XMFLOAT3 GetPosition() const { return mPos; };
	DirectX::XMVECTOR GetPositionXM() const { return DirectX::XMLoadFloat3(&mPos); };

	/// @brief 右方向の軸
	/// @return 
	DirectX::XMFLOAT3 GetRightAxis() const;
	DirectX::XMVECTOR GetRightAxisXM() const;

    /// @brief 上方向の軸
    /// @return 
    DirectX::XMFLOAT3 GetUpAxis() const;
    DirectX::XMVECTOR GetUpAxisXM() const;

	/// @brief 前方向の軸
	/// @return 
    DirectX::XMFLOAT3 GetForwardAxis() const;
    DirectX::XMVECTOR GetForwardAxisXM() const;

    DirectX::XMFLOAT4X4 GetLocalToWorldMatrix() const;
    DirectX::XMMATRIX GetLocalToWorldMatrixXM() const;

    DirectX::XMFLOAT4X4 GetWorldToLocalMatrix() const;
    DirectX::XMMATRIX GetWorldToLocalMatrixXM() const;

    
    void SetScale(const DirectX::XMFLOAT3& scale);
    void SetScale(float x, float y, float z);

	/// @brief Set Rotation in Degree
	/// @param eulerAnglesInDegree 角度
	void SetRotationInDegree(const DirectX::XMFLOAT3& eulerAnglesInDegree);

	/// @brief Set Rotation in Radian
	///	回転の順序：Z-X-Y
	/// @param eulerAnglesInRadian ラジアン
	void SetRotationInRadian(const DirectX::XMFLOAT3& eulerAnglesInRadian);

	/// @brief Set Rotation in eulerAnglesInRadian
	/// 回転の順序：Z-X-Y
    void SetRotation(float x, float y, float z);
	void SetRotationInDegree(float x, float y, float z);

    void SetPosition(const DirectX::XMFLOAT3& position);
    void SetPosition(float x, float y, float z);
	void SetPosition(float* pos);
	void SetPositionZ(float z);

	/// @brief Rotate given the degree
	/// @param eulerAnglesInDegree 回転角度
	void Rotate(const DirectX::XMFLOAT3& eulerAnglesInDegree);

	/// @brief 原点を中心に回転
	/// @param axis 軸(X/Y/Z)
	/// @param degree 角度
	void RotateAxis(const DirectX::XMFLOAT3& axis, float degree);

	/// @brief あるポイントを中心に回転
	/// @param point ポイント位置
	/// @param axis 軸(X/Y/Z)
	/// @param degree 角度
	void RotateAround(const DirectX::XMFLOAT3& point, const DirectX::XMFLOAT3& axis, float degree);

	/// @brief Parallel Move
	/// @param direction 方向
	/// @param magnitude 距離
	void Translate(const DirectX::XMFLOAT3& direction, float magnitude);

	/// @brief ターゲットを観察
	/// @param target ターゲット位置
	/// @param up 上方向
	void LookAt(const DirectX::XMFLOAT3& target, const DirectX::XMFLOAT3& up = { 0.0f, 1.0f, 0.0f });

	/// @brief ある方向に沿ってターゲットを観察
	/// @param direction 方向
	/// @param up 上方向
	void LookTo(const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT3& up = { 0.0f, 1.0f, 0.0f });

	/// @brief 回転行列からEulerAngleを取得 
	/// @param rotationMatrix 回転行列
	/// @return EulerAngle
	DirectX::XMFLOAT3 GetEulerAnglesFromRotationMatrix(const DirectX::XMFLOAT4X4& rotationMatrix);

private:
	/// @brief Transformデータを行列に変換する
	DirectX::XMFLOAT4X4 GenerateWorld() const;
};

