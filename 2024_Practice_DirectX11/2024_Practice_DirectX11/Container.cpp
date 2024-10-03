#include "Container.h"
#include "GampApp.h"
#include "GUI.h"
#include "RenderState.h"

Container::Container()
{
}

void Container::Update(float dt)
{
#ifdef _DEBUG

	GUI::ObjectSetting(this, "Container");

#endif

	//Render Update
	Object::LateUpdate(dt);
}

void Container::Draw()
{
	Object::Draw();
}

