#pragma once
#include "StaticObject.h"

namespace ObjectMovement
{
	enum Movement
	{
		STATE_NONE,		//�Î~
		STATE_SPIN_IN,	//��]����
		STATE_PITCH,	//�㉺�ړ�
		STATE_WALK,		//�O�㍶�E
	};

	enum Axis
	{
		AxisX,		//YZ���ʂ̉�]
		AxisY,		//XZ���ʂ̉�]
		AxisZ,		//XY���ʂ̉�]
	};

	
}

namespace Ease
{
	enum EasingMode
	{
		MODE_NONE,
		MODE_SINE,
		MODE_BACK,
		MODE_BOUNCE,
		MODE_CIRC,

	};

	inline float EaseOutSine(float ratio)
	{
		return std::sin(ratio * DirectX::XM_PI / 2.0f);
	}

	inline float EaseOutBack(float ratio)
	{
		const float c1 = 1.70158f;
		const float c3 = c1 + 1;

		return (float)(1 + c3 * std::pow(ratio - 1, 3) + c1 * std::pow(ratio - 1, 2));
	}

	inline float EaseOutBounce(float ratio)
	{
		const float n1 = 7.5625f;
		const float d1 = 2.75f;

		if (ratio < 1 / d1)
		{
			return n1 * ratio * ratio;
		}
		else if (ratio < 2 / d1)
		{
			return n1 * (ratio -= 1.5f / d1) * ratio + 0.75f;
		}
		else if (ratio < 2.5f / d1)
		{
			return n1 * (ratio -= 2.25f / d1) * ratio + 0.9375f;
		}
		else
		{
			return n1 * (ratio -= 2.625f / d1) * ratio + 0.984375f;
		}
	}

	inline float EaseOutCirc(float ratio)
	{
		return  (float)std::sqrt(1 - pow(ratio - 1, 2));
	}

	inline float EaseInCubic(float ratio)
	{
		return ratio * ratio * ratio;
		
	}

	inline float EaseInBack(float ratio)
	{
		const float c1 = 1.70158f;
		const float c3 = c1 + 1;

		return c3 * ratio * ratio * ratio - c1 * ratio * ratio;
	}

	inline float EaseOutCubic(float ratio)
	{
		return (float)(1.f - std::pow(1.f - ratio, 3));
	}

	inline float EaseInExpo(float ratio)
	{
		return (ratio == 0.0f) ? 0.0f : std::pow(2.0f, 10.0f * ratio - 10.0f);
	}
	
}

class MovableStaticObject :public StaticObject
{

	float mAccumulateTime = 0.0f;		//�݌v����
	float mDuration = 0.0f;
	//�ړ�����
	DirectX::XMFLOAT3 mStartPos = {};		//�ړ��̏����ʒu
	DirectX::XMFLOAT3 mStartRotation = {};	//��]�̏����p�x

	DirectX::XMFLOAT3 mRotateAmount = {};	//��]��

	DirectX::XMFLOAT3 mSpinCenter = {};		//��]���S
	float mSpinDegree = 0.0f;				//��]�^����
	int mAxis = 0;

	DirectX::XMFLOAT3 mDistance = {};	//�ړ���

	int mState = 0;
	int mNextState = 0;					//���̃X�e�[�g
	int mEaseMode = 0;

	bool isFinishMovement = false;

public:
	MovableStaticObject() {};
	~MovableStaticObject() override = default;

	virtual void Update(float dt) override;
	virtual void GameUpdate(float dt);

	/// @brief ��]����̃f�[�^�ݒ�
	/// @param spinDegree ��]�^���p�x
	///	@param spinCenter ��]���S
	///	@param startPos �I�_�ʒu
	/// @param rotation ��]��
	///	@param startRotation ������]���
	/// @param duration �^������
	/// @param axis ��]��
	///	@param mode �C�[�W���O���[�h
	virtual void SetSpinIn(float spinDegree, const DirectX::XMFLOAT3& spinCenter, const DirectX::XMFLOAT3& startPos, const DirectX::XMFLOAT3& rotation, const DirectX::XMFLOAT3& startRotation, float duration, ObjectMovement::Axis axis, Ease::EasingMode mode);

	virtual void SetWalk(const DirectX::XMFLOAT3& distance, float duration, const DirectX::XMFLOAT3& startPos, Ease::EasingMode mode, ObjectMovement::Axis axis);

	/// ��ԕύX
	virtual void SetMoveState(ObjectMovement::Movement state);

	/// @brief Easing Mode��ݒ肷��
	/// @param mode Easing���
	virtual void SetEasingMode(Ease::EasingMode mode);

	/// @brief �C�[�W���O����
	/// @param data  Normalized data
	/// @return float(0~1)
	float Ease(float data);

	void SpinIn(float dt);
	void Pitch(float dt);

	void Walk(float dt);

	ObjectMovement::Movement GetNextState();

	bool GetMovementFinished();

	virtual void OnStateNone() {};
};

