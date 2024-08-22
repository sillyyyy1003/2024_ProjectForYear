#include "DirLight.h"
#include "KInput.h"

void DirLight::Update(float dt)
{
	if (!KInput::IsKeyPress(VK_SHIFT))return;

	if(KInput::IsKeyPress(VK_LEFT))mPos.x -= 3.0f * dt;
	if(KInput::IsKeyPress(VK_RIGHT))mPos.x += 3.0f * dt;
	if (KInput::IsKeyPress(VK_UP))mPos.y += 3.0f * dt;
	if (KInput::IsKeyPress(VK_DOWN))mPos.y -= 3.0f * dt;



}
