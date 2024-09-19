#pragma once
#include "Object.h"

class Container :public Object
{
public:
	Container();
	~Container() override = default;

	void Update(float dt) override;
	void Draw() override;

};

