#pragma once
#include "Collider.h"
#include "Cube.h"
#include "D3DUtil.h"
#include "PBRModel.h"
#include "Primitive.h"
#include "StaticObject.h"

class InteractiveMovableObject:public StaticObject
{
protected:

	struct RimLightEffect
	{
		DirectX::XMFLOAT3 rimColor = { 1.f,1.f,1.f };
		float rimIntensity = 0.0f;
	};

	// Used for clicked or Hover
	RimLightEffect mEffect;
	DirectX::XMFLOAT3 mColliderExtents = { 0,0,0 };

protected:

	std::unique_ptr<BoxCollider> mCollider = nullptr;

	//Trigger used to update collider
	bool isModelStateChange = false;

	// object state
	int mObjectState = 0;
	bool isClicked = false;
	bool isDragged = false;

	POINT mOldPos = { 0,0 };
	//操作できるか？
	bool isControllable = true;

	//Debug用
#ifdef _DEBUG
	std::unique_ptr<Cube> mDebugColliderMesh = nullptr;
	bool isShowCollider = false;
#endif



public:
	InteractiveMovableObject();
	~InteractiveMovableObject() override = default;

	virtual void InitModel(const char* filePath, const char* _objName, PrimitiveConfig::PrimitiveKind _kind, DirectX::XMINT2 _UVSplit) override;
	virtual void InitPBR(const char* filePath, const char* _objName) override;


	/// @brief 
	/// @param dt delta time
	virtual void Update(float dt);

	virtual void Draw();

	bool GetClicked();

	//====================================
	//	外側でモデルを設定できるのは以下のみ
	//====================================
	virtual void SetModelPosition(DirectX::XMFLOAT3 pos);
	virtual void SetModelRotation(DirectX::XMFLOAT3 rot);
	virtual void SetModelScale(DirectX::XMFLOAT3 scale);

	virtual void SetModelPosition(const float* position);
	virtual void SetModelRotation(const float* rot);
	virtual void SetModelScale(const float* scale);


	virtual json SaveData() override;
	virtual void LoadSaveData(json data) override;

	PixelShader* GetDefPS() const { return mModel->GetDefPS(); };


	virtual void InitCollider();
	virtual void TriggerListener();
	virtual void PreUpdate(float dt);
	virtual void GameUpdate(float dt);
	virtual void LateUpdate(float dt);
	virtual void UpdateCollider();

	/// @brief 状態変化をListenerを通知する
	virtual void NotifyModelStateChangeListener();
	/// @brief 状態変化をリセットする
	virtual void ClearModelStateChangeListener();
	virtual bool GetModelStateChange() { return isModelStateChange; };

	virtual void SetDiffuse(DirectX::XMFLOAT4 color);


	virtual void OnStateNone(float dt);
	virtual void OnStateHover(float dt);
	virtual void OnStateClicked(float dt);
	virtual void OnStateDrag(float dt);
	virtual void OnStateReleased(float dt);
};

