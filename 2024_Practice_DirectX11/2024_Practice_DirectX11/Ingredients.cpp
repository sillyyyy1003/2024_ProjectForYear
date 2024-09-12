#include "Ingredients.h"
#include "Cube.h"
#include "Capsule.h"
#include "Cylinder.h"
#include "Plane3D.h"
#include "Sphere.h"

Ingredients::Ingredients()
{
}

void Ingredients::InitPrimitive(PrimitiveKind kind)
{
	switch(kind)
	{
	case CUBE:
		mMesh = std::make_unique<Cube>();
		break;
	case CAPSULE:
		mMesh = std::make_unique<Capsule>();
		break;
	case CYLINDER:
		mMesh = std::make_unique<Cylinder>();
		break;
	case PLANE:
		mMesh = std::make_unique<Plane3D>();
		break;
	case SPHERE:
		mMesh = std::make_unique<Sphere>();
		break;
	}
}

void Ingredients::InitTex(const char* filePath)
{
	mMesh->Init(filePath);
}

void Ingredients::Update(float dt)
{
}

void Ingredients::Draw()
{
}

void Ingredients::Drag()
{
}
