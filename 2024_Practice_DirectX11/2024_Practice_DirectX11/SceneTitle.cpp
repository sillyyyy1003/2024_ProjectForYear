#include <nlohmann/json.hpp>
#include <fstream>
#include "SceneTitle.h"
#include "FirstPersonCamera.h"
#include "CanvasUI.h"
#include "DirLight.h"

void SceneTitle::Init()
{
	CanvasUI* bg = CreateObj<CanvasUI>("TitleBg");
	CanvasUI* startButton = CreateObj<CanvasUI>("StartButton");
	CanvasUI* settingButton = CreateObj<CanvasUI>("SettingButton");
	CanvasUI* exitButton = CreateObj<CanvasUI>("ExitButton");
	FirstPersonCamera* titleCamera = CreateObj<FirstPersonCamera>("TitleCamera");

	//Setting
	bg->Init("");







}

void SceneTitle::UnInit()
{
	
}

void SceneTitle::Update(float dt)
{
}

void SceneTitle::Draw()
{
}
