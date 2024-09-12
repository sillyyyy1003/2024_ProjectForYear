#pragma once
#include "CanvasUI.h"
#include "SceneBase.h"
#include "Primitive.h"

class SceneBlank:public SceneBase
{
public:
	void Init();
	void UnInit();
	void Update(float dt);
	void Draw();

private:

	std::unique_ptr<Primitive> primitive = nullptr;
	std::unique_ptr<Primitive> floor = nullptr;

	std::unique_ptr<CanvasUI> uiBg = nullptr;
	std::unique_ptr<CanvasUI> uiStart = nullptr;
	std::unique_ptr<CanvasUI> uiOption = nullptr;
	std::unique_ptr<CanvasUI> uiExit = nullptr;
};
