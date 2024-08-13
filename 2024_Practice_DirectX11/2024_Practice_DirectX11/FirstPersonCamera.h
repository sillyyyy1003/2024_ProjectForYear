#pragma once
#include "CameraBase.h"
#include "Transform.h"

/// @brief 一人称カメラ
class FirstPersonCamera : public CameraBase
{
private:
    //Transform mTransform = {};
    float mMoveSpeed = 3.0f;

    int mState = 0;
    POINT mOldPos = { 0,0 };

public:

    FirstPersonCamera() = default;
    ~FirstPersonCamera() override = default;

    void Update(float dt) override;

    /// @brief カメラの位置設定
    void SetPosition(float x, float y, float z);
    void SetPosition(const DirectX::XMFLOAT3& pos);

    /// @brief カメラの向きを設定
    /// @param pos カメラの位置
    /// @param target ターゲットの位置
    /// @param up 上方向
    void LookAt(const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& target, const DirectX::XMFLOAT3& up);

    /// @brief ある方向に沿って観察
    /// @param pos カメラの位置
    /// @param to 方向
    /// @param up 上方向
    void LookTo(const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& to, const DirectX::XMFLOAT3& up);

    /// @brief 平行移動
    /// @param d 距離
    void Strafe(float d);

    /// @brief 直行
    /// @param d 距離
    void Walk(float d);

    /// @brief 前に直行
    /// @param d 距離
    void MoveForward(float d);

    /// @brief 上下観察
    /// @param rad +:UP -:DOWN
    void Pitch(float rad);

    /// @brief 左右観察
    /// @param rad +:LEFT -:RIGHT
    void RotateY(float rad);

    void MoveUpward(float d);

private:
    void UpdateState();
    void UpdateFlight(DirectX::XMFLOAT2 mouseMove, float dt);

};

