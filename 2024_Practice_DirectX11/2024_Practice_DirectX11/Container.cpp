#include "Container.h"

#include "GampApp.h"
#include "RenderState.h"

Container::Container()
{
}

void Container::Update(float dt)
{
	//Object::Update(dt);
}

void Container::Draw()
{
	//���\���ʕ`�悷��
	GameApp::SetCullingMode(RenderState::RSNoCull);
	//�����x��ݒ�
	GameApp::SetBlendState(RenderState::BSTransparent);

	Object::Draw();
}