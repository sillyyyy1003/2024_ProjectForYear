#include "D2D_UIBase.h"

D2D_UIBase::D2D_UIBase(D2DUIConfig::UIShape shape)
{
	mShape = shape;
}

json D2D_UIBase::SaveData()
{
	json data;
	data["Position"] = { mPos.x,mPos.y };
	data["Scale"] = { mScale.x,mScale.y };
	data["BaseColor"] = { mBaseColor.r,mBaseColor.g,mBaseColor.b,mBaseColor.a };
	data["OutlineWeight"] = { mOutlineWeight };
	data["OutlineColor"] = { mOutlineColor.r,mOutlineColor.g,mOutlineColor.b,mOutlineColor.a };
	data["RadiusX"] = { mRadiusX };
	data["RadiusY"] = { mRadiusY };
	return data;
}

void D2D_UIBase::LoadSaveData(json data)
{
	mPos = { data["Position"][0],data["Position"][1] };
	mScale= { data["Scale"][0],data["Scale"][1] };
	mBaseColor = { data["BaseColor"][0] ,data["BaseColor"][1] ,data["BaseColor"][2] ,data["BaseColor"][3] };
	mOutlineColor = { data["OutlineColor"][0] ,data["OutlineColor"][1] ,data["OutlineColor"][2] ,data["OutlineColor"][3] };
	mOutlineWeight = { data["OutlineWeight"][0]};
	mRadiusX = { data["RadiusX"][0] };
	mRadiusY = { data["RadiusY"][0] };
}

void D2D_UIBase::LoadSaveData(json data, const char* objName)
{
	mPos = { data[objName]["Position"][0],data[objName]["Position"][1] };
	mScale = { data[objName]["Scale"][0],data[objName]["Scale"][1] };
	mBaseColor = { data[objName]["BaseColor"][0] ,data[objName]["BaseColor"][1] ,data[objName]["BaseColor"][2] ,data[objName]["BaseColor"][3] };
	mOutlineColor = { data[objName]["OutlineColor"][0] ,data[objName]["OutlineColor"][1] ,data[objName]["OutlineColor"][2] ,data[objName]["OutlineColor"][3] };
	mOutlineWeight = { data[objName]["OutlineWeight"][0] };
	mRadiusX = { data[objName]["RadiusX"][0] };
	mRadiusY = { data[objName]["RadiusY"][0] };
}

DirectX::XMFLOAT4 D2D_UIBase::GetBackGroundColor()
{
	return {
	mBaseColor.r,
	mBaseColor.g,
	mBaseColor.b,
	mBaseColor.a
	};
}

DirectX::XMFLOAT4 D2D_UIBase::GetOutlineColor()
{
	return {
		mOutlineColor.r,
		mOutlineColor.g,
		mOutlineColor.b,
		mOutlineColor.a
	};
}
