#pragma once
#include "CameraBase.h"
class ThirdPersonCamera : public CameraBase
{
private:

    int mState = 0;
    POINT mOldPos = { 0,0 };
    float mMoveSpeed = 3.0f;

public:
    DirectX::XMFLOAT3 mTarget={};

    //ターゲットとの距離
    float mDistance = 0.0f;

    //最短許容距離
    float mMinDist = 0.0f;

    //最大許容距離
	float mMaxDist = 0.0f;

    ThirdPersonCamera() = default;
    ~ThirdPersonCamera() override;

    void Update(float dt) override;

    /// @brief ターゲットとの位置を取得
    /// @return ターゲットの位置
    DirectX::XMFLOAT3 GetTargetPosition() const;

    /// @brief オブジェクトとの距離
    float GetDistance() const;

    /// @brief オブジェクトを中心に垂直回転(0~60)
    /// @param rad ラジアン
    void RotateX(float rad);

    /// @brief オブジェクトを中心に水平回転(0~60)
    /// @param rad ラジアン
    void RotateY(float rad);

    /// @brief オブジェクトに近付く
    /// @param dist 
    void Approach(float dist);

    /// @brief 垂直回転の初期設定[0, pi/3]
    /// @param rad ラジアン
    void SetRotationX(float rad);

    /// @brief 水平回転の初期設定[0, pi/3]
	/// @param rad ラジアン
    void SetRotationY(float rad);

    /// @brief 追跡対象を設定
    /// @param target 
    void SetTarget(const DirectX::XMFLOAT3& target);

	/// @brief 追跡距離を設定
    /// @param dist 距離
    void SetDistance(float dist);

    /// @brief 距離の最大最小値を設定
    /// @param minDist 
    /// @param maxDist 
    void SetDistanceMinMax(float minDist, float maxDist);

private:

    void UpdateState();

    void UpdateDolly(DirectX::XMFLOAT2 mouseMove, float dt);

   
};

