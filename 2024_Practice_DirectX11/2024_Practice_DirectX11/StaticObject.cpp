#include "StaticObject.h"
#include <memory>
#include "Capsule.h"
#include "Circle.h"
#include "Cylinder.h"
#include "CylinderOneCap.h"
#include "GUI.h"
#include "Model.h"
#include "Sphere.h"
#include "Square.h"

StaticObject::StaticObject()
{
}

void StaticObject::InitPBR(const char* filePath, const char* _objName)
{
	mModel = std::make_unique<PBRModel>();
	mModel->InitWithoutTex(filePath);
	mObjectName = _objName;
	isUsePBR = true;
}


void StaticObject::InitModel(const char* filePath, const char* _objName, PrimitiveConfig::PrimitiveKind _kind, DirectX::XMINT2 _UVSplit)
{
	switch(_kind)
	{
	default:
	case PrimitiveConfig::CAPSULE:
		mModel = std::make_unique<Capsule>();
		mModel->Init(filePath,PrimitiveConfig::DEFAULT_MESH_SLICES, PrimitiveConfig::DEFAULT_MESH_SLICES, _UVSplit);
		break;
	case PrimitiveConfig::CUBE:
		mModel = std::make_unique<Cube>();
		mModel->Init(filePath, _UVSplit);
		break;
	case PrimitiveConfig::SPHERE:
		mModel = std::make_unique<Sphere>();
		mModel->Init(filePath, PrimitiveConfig::DEFAULT_MESH_SLICES, PrimitiveConfig::DEFAULT_MESH_SLICES, _UVSplit);
		break;
	case PrimitiveConfig::CYLINDER:
		mModel = std::make_unique<Cylinder>();
		mModel->Init(filePath, PrimitiveConfig::DEFAULT_MESH_SLICES, PrimitiveConfig::DEFAULT_MESH_SLICES, _UVSplit);
		break;
	case PrimitiveConfig::CYLINDER_ONECAP:
		mModel = std::make_unique<CylinderOneCap>();
		mModel->Init(filePath, PrimitiveConfig::DEFAULT_MESH_SLICES, PrimitiveConfig::DEFAULT_MESH_SLICES, _UVSplit);
		break;
	case PrimitiveConfig::SQUARE:
		mModel = std::make_unique<Square>();
		mModel->Init(filePath, PrimitiveConfig::DEFAULT_MESH_SLICES, _UVSplit);
		break;
	case PrimitiveConfig::CIRCLE:
		mModel = std::make_unique<Circle>();
		mModel->Init(filePath, PrimitiveConfig::DEFAULT_MESH_SLICES, PrimitiveConfig::DEFAULT_MESH_SLICES, _UVSplit);
		break;
	case PrimitiveConfig::MULTI:
		mModel = std::make_unique<Model>();
		mModel->Init(filePath);
		break;
	}
	mObjectName = _objName;
	isUsePBR = false;
}

void StaticObject::InitModel(const std::shared_ptr<Texture>& tex, const char* _objName, PrimitiveConfig::PrimitiveKind _kind,
	DirectX::XMINT2 _UVSplit)
{
	switch (_kind)
	{
	default:
	case PrimitiveConfig::CAPSULE:
		mModel = std::make_unique<Capsule>();
		mModel->Init(tex, PrimitiveConfig::DEFAULT_MESH_SLICES, PrimitiveConfig::DEFAULT_MESH_SLICES, _UVSplit);
		break;
	case PrimitiveConfig::CUBE:
		mModel = std::make_unique<Cube>();
		mModel->Init(tex, _UVSplit);
		break;
	case PrimitiveConfig::SPHERE:
		mModel = std::make_unique<Sphere>();
		mModel->Init(tex, PrimitiveConfig::DEFAULT_MESH_SLICES, PrimitiveConfig::DEFAULT_MESH_SLICES, _UVSplit);
		break;
	case PrimitiveConfig::CYLINDER:
		mModel = std::make_unique<Cylinder>();
		mModel->Init(tex, PrimitiveConfig::DEFAULT_MESH_SLICES, PrimitiveConfig::DEFAULT_MESH_SLICES, _UVSplit);
		break;
	case PrimitiveConfig::CYLINDER_ONECAP:
		mModel = std::make_unique<CylinderOneCap>();
		mModel->Init(tex, PrimitiveConfig::DEFAULT_MESH_SLICES, PrimitiveConfig::DEFAULT_MESH_SLICES, _UVSplit);
		break;
	case PrimitiveConfig::SQUARE:
		mModel = std::make_unique<Square>();
		mModel->Init(tex, PrimitiveConfig::DEFAULT_MESH_SLICES, _UVSplit);
		break;
	case PrimitiveConfig::CIRCLE:
		mModel = std::make_unique<Circle>();
		mModel->Init(tex, PrimitiveConfig::DEFAULT_MESH_SLICES, PrimitiveConfig::DEFAULT_MESH_SLICES, _UVSplit);
		break;
	}
	mObjectName = _objName;
	isUsePBR = false;
}

void StaticObject::LoadTex(PBRConfig::PBRTexList list)
{
	mModel->LoadTex(list);
}

void StaticObject::LoadShaderFile(const std::shared_ptr<VertexShader>& vs, const std::shared_ptr<PixelShader>& ps)
{
	mModel->Primitive::LoadDefShader(vs, ps);
}

void StaticObject::LoadShaderFile(const char* vsFile, const char* psFile)
{
	mModel->Primitive::LoadDefShader(vsFile, psFile);
}

void StaticObject::Update(float dt)
{
#ifdef _DEBUG

	if (ImGui::Begin(mObjectName.c_str()))
	{
		float scale[3] = { mModel->GetScale().x,mModel->GetScale().y,mModel->GetScale().z };
		ImGui::InputFloat3("Scale", scale);
		mModel->SetScale(scale);

		float pos[3] = { mModel->GetPosition().x,mModel->GetPosition().y,mModel->GetPosition().z };
		ImGui::InputFloat3("Position", pos);
		mModel->SetPosition(pos);

		ImGui::InputFloat3("Rotation", rot);
		if(ImGui::Button("SetRot"))
		{
			mModel->SetRotation(rot);
		}

		GUI::ShowFloat3(mModel->GetRotation());
	}
	ImGui::End();
#endif
	mModel->Update(dt);
}

void StaticObject::Draw()
{
	
	mModel->Draw();
}       


void StaticObject::LoadSaveData(json data)
{
	Vector3 pos = Vector3(data[mObjectName.c_str()]["Position"][0], data[mObjectName.c_str()]["Position"][1], data[mObjectName.c_str()]["Position"][2]);
	mModel->SetPosition(pos);

	Vector3 scale = Vector3(data[mObjectName.c_str()]["Scale"][0], data[mObjectName.c_str()]["Scale"][1], data[mObjectName.c_str()]["Scale"][2]);
	mModel->SetScale(scale);

	Vector3 rot = Vector3(data[mObjectName.c_str()]["Rotation"][0], data[mObjectName.c_str()]["Rotation"][1], data[mObjectName.c_str()]["Rotation"][2]);
	mModel->mTransform.SetRotationInRadian(rot);
}

json StaticObject::SaveData()
{
	json data;
	data["Position"] = { mModel->GetPosition().x,mModel->GetPosition().y,mModel->GetPosition().z };
	data["Scale"] = { mModel->GetScale().x,mModel->GetScale().y,mModel->GetScale().z };
	data["Rotation"] = { mModel->GetRotation().x,mModel->GetRotation().y,mModel->GetRotation().z };
	
	return data;

}

void StaticObject::LoadDefShader(const std::shared_ptr<VertexShader>& vs, const std::shared_ptr<PixelShader>& ps)
{
	mModel->LoadDefShader(vs, ps);
}

void StaticObject::SetTransparency(float _transparency)
{
	mModel->GetMaterial().diffuse.w = _transparency;
}
