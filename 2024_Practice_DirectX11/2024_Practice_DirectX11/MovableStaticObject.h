#pragma once
#include "StaticObject.h"

namespace ObjectMovement
{
	enum Movement
	{
		STATE_NONE,		//静止
		STATE_SPIN_IN,	//回転入り
		STATE_PITCH,	//上下移動
		STATE_WALK,		//前後左右
	};

	enum Axis
	{
		AxisX,		//YZ平面の回転
		AxisY,		//XZ平面の回転
		AxisZ,		//XY平面の回転
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

	float mAccumulateTime = 0.0f;		//累計時間
	float mDuration = 0.0f;
	//移動時間
	DirectX::XMFLOAT3 mStartPos = {};		//移動の初期位置
	DirectX::XMFLOAT3 mStartRotation = {};	//回転の初期角度

	DirectX::XMFLOAT3 mRotateAmount = {};	//回転量

	DirectX::XMFLOAT3 mSpinCenter = {};		//回転中心
	float mSpinDegree = 0.0f;				//回転運動量
	int mAxis = 0;

	DirectX::XMFLOAT3 mDistance = {};	//移動量

	int mState = 0;
	int mNextState = 0;					//次のステート
	int mEaseMode = 0;

	bool isFinishMovement = false;

public:
	MovableStaticObject() {};
	~MovableStaticObject() override = default;

	virtual void Update(float dt) override;
	virtual void GameUpdate(float dt);

	/// @brief 回転入りのデータ設定
	/// @param spinDegree 回転運動角度
	///	@param spinCenter 回転中心
	///	@param startPos 終点位置
	/// @param rotation 回転量
	///	@param startRotation 初期回転情報
	/// @param duration 運動時間
	/// @param axis 回転軸
	///	@param mode イージングモード
	virtual void SetSpinIn(float spinDegree, const DirectX::XMFLOAT3& spinCenter, const DirectX::XMFLOAT3& startPos, const DirectX::XMFLOAT3& rotation, const DirectX::XMFLOAT3& startRotation, float duration, ObjectMovement::Axis axis, Ease::EasingMode mode);

	virtual void SetWalk(const DirectX::XMFLOAT3& distance, float duration, const DirectX::XMFLOAT3& startPos, Ease::EasingMode mode, ObjectMovement::Axis axis);

	/// 状態変更
	virtual void SetMoveState(ObjectMovement::Movement state);

	/// @brief Easing Modeを設定する
	/// @param mode Easing種類
	virtual void SetEasingMode(Ease::EasingMode mode);

	/// @brief イージング効果
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

