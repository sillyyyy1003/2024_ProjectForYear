#pragma once
#include "CanvasUI.h"
#include "FirstPersonCamera.h"
#include "Model.h"
#include "Plane3D.h"
#include "SceneBase.h"

class SceneBlank:public SceneBase
{
public:
	void Init();
	void UnInit();
	void Update(float dt);
	void Draw();

private:
	//std::unique_ptr<Box3D> box;
	std::unique_ptr<Plane3D> bg[3];
	std::unique_ptr<Model> model;
	std::unique_ptr<FirstPersonCamera> firstCamera;
	std::unique_ptr<CanvasUI> ui;

};
