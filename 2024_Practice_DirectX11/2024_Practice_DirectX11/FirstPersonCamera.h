#pragma once
#include "CameraBase.h"
#include "Transform.h"

/// @brief ��l�̃J����
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

    /// @brief �J�����̈ʒu�ݒ�
    void SetPosition(float x, float y, float z);
    void SetPosition(const DirectX::XMFLOAT3& pos);

    /// @brief �J�����̌�����ݒ�
    /// @param pos �J�����̈ʒu
    /// @param target �^�[�Q�b�g�̈ʒu
    /// @param up �����
    void LookAt(const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& target, const DirectX::XMFLOAT3& up);

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

private:
    void UpdateState();
    void UpdateFlight(DirectX::XMFLOAT2 mouseMove, float dt);

};
