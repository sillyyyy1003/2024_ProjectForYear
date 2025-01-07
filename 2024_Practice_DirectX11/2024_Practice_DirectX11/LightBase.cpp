#include "LightBase.h"
#include <SimpleMath.h>
using namespace DirectX::SimpleMath;

LightBase::LightBase()
{
}

void LightBase::SetPosition(const DirectX::XMFLOAT3& _pos)
{
	mPos = _pos;
}

void LightBase::SetPosition(float* _pos)
{
	mPos = Vector3(_pos[0], _pos[1], _pos[2]);
}

void LightBase::SetDirection(const DirectX::XMFLOAT3& _dir)
{
	mDir = _dir;
}

void LightBase::SetDirection(float* _dir)
{
	mDir = Vector3(_dir[0], _dir[1], _dir[2]);
}

void LightBase::SetAmbient(const DirectX::XMFLOAT4& _ambient)
{
	mAmbient = _ambient;
}

void LightBase::SetAmbient(float* ambient)
{
	mAmbient = Color(ambient[0], ambient[1], ambient[2], ambient[3]);
}

void LightBase::SetDiffuse(const DirectX::XMFLOAT4& _diffuse)
{
	mDiffuse = _diffuse;
}

void LightBase::SetDiffuse(float* diffuse)
{
	mDiffuse = Color(diffuse[0], diffuse[1], diffuse[2], diffuse[3]);
}
