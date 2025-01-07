#pragma once
#include "CameraBase.h"
#include "Transform.h"

/// @brief 一人称カメラ
class FirstPersonCamera : public CameraBase
{
private:
 
    float mMoveSpeed = 5.0f;    //移動速度

    int mState = 0;
    POINT mOldPos = { 0,0 };

    bool isLockPos = false;     // 位置移動可能？
    bool isLockAngle = false;   // 角度固定？

    //UpdateMoveで使われる変数
    DirectX::XMFLOAT3 mTargetPosition={};
    DirectX::XMFLOAT3 mTargetRotation={};
    DirectX::XMFLOAT3 mDirection = {};
    DirectX::XMFLOAT3 mDefaultPosition = {};
    float mDistance = 0.0f;         //ターゲットとの距離
  
    float mAccumulateTime = 0.f;    // 動きの累積時間
    float mDuration = 1.f;
    float mRotateSpeed = 0.5f;       // 回転速度
 
    bool isMoveToTarget = false;    // 回転運動するかどうか

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


    void LookAt(const DirectX::XMFLOAT3& target);

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

    void LookDown();

    /// @brief 上下観察
    /// @param rad +:UP -:DOWN
    void Pitch(float rad);

    /// @brief 左右観察
    /// @param rad +:LEFT -:RIGHT
    void RotateY(float rad);

    void MoveUpward(float d);

    /// @brief カメラの角度をロックする
    void LockCameraAngle(bool isLockAngle) { this->isLockAngle = isLockAngle; };
    bool GetAngleLock() { return isLockAngle; };

    /// @brief カメラの位置をロックする
    void LockCameraPos(bool isLockPos) { this->isLockPos = isLockPos; };
    bool GetPosLock() { return isLockPos; };

    json SaveData();
    void LoadSaveData(json data, const char* objName);

    void LockCamera();

    /// @brief 
	/// @param targetPos 指定位置
    /// @param targetRot 指定角度
    /// @param duration 移動時間
    void StartMoveToTarget(DirectX::XMFLOAT3 targetPos, DirectX::XMFLOAT3 targetRot, float duration);

    void BackToDefaultPos();

    /// @brief 特定位置をアップする
    /// @param dt 
    void ZoomIn(float dt);


private:
    void UpdateState();
    void UpdateFlight(DirectX::XMFLOAT2 mouseMove, float dt);

	/// @brief 指定位置・角度に移動する
    void UpdateMove(float dt);//todo:need to fix the logic problems


};

