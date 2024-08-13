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

    //�^�[�Q�b�g�Ƃ̋���
    float mDistance = 0.0f;

    //�ŒZ���e����
    float mMinDist = 0.0f;

    //�ő勖�e����
	float mMaxDist = 0.0f;

    ThirdPersonCamera() = default;
    ~ThirdPersonCamera() override;

    void Update(float dt) override;

    /// @brief �^�[�Q�b�g�Ƃ̈ʒu���擾
    /// @return �^�[�Q�b�g�̈ʒu
    DirectX::XMFLOAT3 GetTargetPosition() const;

    /// @brief �I�u�W�F�N�g�Ƃ̋���
    float GetDistance() const;

    /// @brief �I�u�W�F�N�g�𒆐S�ɐ�����](0~60)
    /// @param rad ���W�A��
    void RotateX(float rad);

    /// @brief �I�u�W�F�N�g�𒆐S�ɐ�����](0~60)
    /// @param rad ���W�A��
    void RotateY(float rad);

    /// @brief �I�u�W�F�N�g�ɋߕt��
    /// @param dist 
    void Approach(float dist);

    /// @brief ������]�̏����ݒ�[0, pi/3]
    /// @param rad ���W�A��
    void SetRotationX(float rad);

    /// @brief ������]�̏����ݒ�[0, pi/3]
	/// @param rad ���W�A��
    void SetRotationY(float rad);

    /// @brief �ǐՑΏۂ�ݒ�
    /// @param target 
    void SetTarget(const DirectX::XMFLOAT3& target);

	/// @brief �ǐՋ�����ݒ�
    /// @param dist ����
    void SetDistance(float dist);

    /// @brief �����̍ő�ŏ��l��ݒ�
    /// @param minDist 
    /// @param maxDist 
    void SetDistanceMinMax(float minDist, float maxDist);

private:

    void UpdateState();

    void UpdateDolly(DirectX::XMFLOAT2 mouseMove, float dt);

   
};

