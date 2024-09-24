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
	//— •\—¼–Ê•`‰æ‚·‚é
	GameApp::SetCullingMode(RenderState::RSNoCull);
	//“§–¾“x‚ğİ’è
	GameApp::SetBlendState(RenderState::BSTransparent);

	Object::Draw();
}
