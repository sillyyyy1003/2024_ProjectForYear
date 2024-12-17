#pragma once
#include "SceneBase.h"
#include "UIStackContainer.h"

namespace UI_MoveObjectConfig
{
	enum class UiMoveState :unsigned int
	{
		STATE_NONE,
		STATE_HOVER,
		STATE_DRAG,
		STATE_RELEASE,
		STATE_MAX,
	};
}
class UI_MoveObject:public Component
{
protected:

	std::unique_ptr<UIStackContainer> mGraphic;
	UI_MoveObjectConfig::UiMoveState mState=UI_MoveObjectConfig::UiMoveState::STATE_NONE;

	bool isRelease = false;
	

public:
	UI_MoveObject() = default;
	~UI_MoveObject() override = default;

	virtual void Init(const char* _objName, const std::shared_ptr<Texture>& iconTex,const std::shared_ptr<Texture>& fontTex);

	virtual void Update(float dt);

	virtual void Draw();

	virtual void PreUpdate(float dt);
	virtual void GameUpdate(float dt);

	virtual json SaveData();
	virtual void LoadSaveData(json saveData);

	virtual void SetObjName(const char* objName) { mGraphic->SetObjName(objName); };

	virtual void OnStateNone(float dt);;
	virtual void OnStateHover(float dt);
	virtual void OnStateDrag(float dt);
	virtual void OnStateRelease(float dt);;
};

