#include "LightBase.h"

LightBase::LightBase()
{
}

void LightBase::SetPos(DirectX::XMFLOAT3 _pos)
{
	mPos = _pos;
}

void LightBase::SetDir(DirectX::XMFLOAT3 _dir)
{
	mDir = _dir;
}

void LightBase::SetAmbient(DirectX::XMFLOAT4 _ambient)
{
	mAmbient = _ambient;
}

void LightBase::SetDiffuse(DirectX::XMFLOAT4 _diffuse)
{
	mDiffuse = _diffuse;
}
