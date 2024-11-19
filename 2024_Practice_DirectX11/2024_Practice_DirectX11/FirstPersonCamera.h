#pragma once
#include "CameraBase.h"
#include "Transform.h"

/// @brief ��l�̃J����
class FirstPersonCamera : public CameraBase
{
private:
 
    float mMoveSpeed = 5.0f;

    int mState = 0;
    POINT mOldPos = { 0,0 };

    bool isLockPos = false;     // �ʒu�ړ��\�H
    bool isLockAngle = false;   // �p�x�Œ�H

public:

    FirstPersonCamera() = default;
    ~FirstPersonCamera() override = default;

    void Update(float dt) override;

    /// @brief �J�����̈ʒu�ݒ�
    void SetPosition(float x, float y, float z);
    void SetPosition(const DirectX::XMFLOAT3& pos);

    /// @brief �J�����̌�����ݒ�
    /// @param pos �J�����̈ʒu
    /// @param target �^�[�Q�b�g�̈ʒu
    /// @param up �����
    void LookAt(const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& target, const DirectX::XMFLOAT3& up);


    void LookAt(const DirectX::XMFLOAT3& target);

    /// @brief ��������ɉ����Ċώ@
    /// @param pos �J�����̈ʒu
    /// @param to ����
    /// @param up �����
    void LookTo(const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& to, const DirectX::XMFLOAT3& up);

    /// @brief ���s�ړ�
    /// @param d ����
    void Strafe(float d);

    /// @brief ���s
    /// @param d ����
    void Walk(float d);

    /// @brief �O�ɒ��s
    /// @param d ����
    void MoveForward(float d);

    /// @brief �㉺�ώ@
    /// @param rad +:UP -:DOWN
    void Pitch(float rad);

    /// @brief ���E�ώ@
    /// @param rad +:LEFT -:RIGHT
    void RotateY(float rad);

    void MoveUpward(float d);

    /// @brief �J�����̊p�x�����b�N����
    void LockCameraAngle(bool isLockAngle) { this->isLockAngle = isLockAngle; };
    bool GetAngleLock() { return isLockAngle; };

    /// @brief �J�����̈ʒu�����b�N����
    void LockCameraPos(bool isLockPos) { this->isLockPos = isLockPos; };
    bool GetPosLock() { return isLockPos; };

    json SaveData();
    void LoadSaveData(json data, const char* objName);
private:
    void UpdateState();
    void UpdateFlight(DirectX::XMFLOAT2 mouseMove, float dt);

};

