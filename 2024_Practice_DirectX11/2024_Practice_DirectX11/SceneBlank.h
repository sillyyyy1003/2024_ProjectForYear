#pragma once
#include "SceneBase.h"

class VertexShader;
class FirstPersonCamera;
class Plane3D;
class Model;
class Box3D;
class PixelShader;

class SceneBlank:public SceneBase
{
public:
	void Init();
	void UnInit();
	void Update(float dt);
	void Draw();

private:
	//std::unique_ptr<Box3D> box;
	std::shared_ptr<Plane3D> bg[3];
	std::shared_ptr<Model> model;

	std::unique_ptr<FirstPersonCamera> firstCamera;

};
