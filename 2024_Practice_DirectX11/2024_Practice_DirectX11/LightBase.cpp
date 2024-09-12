#include "LightBase.h"

LightBase::LightBase()
{
}

void LightBase::SetPos(const DirectX::XMFLOAT3& _pos)
{
	mPos = _pos;
}

void LightBase::SetDir(const DirectX::XMFLOAT3& _dir)
{
	mDir = _dir;
}

void LightBase::SetAmbient(const DirectX::XMFLOAT4& _ambient)
{
	mAmbient = _ambient;
}

void LightBase::SetDiffuse(const DirectX::XMFLOAT4& _diffuse)
{
	mDiffuse = _diffuse;
}
