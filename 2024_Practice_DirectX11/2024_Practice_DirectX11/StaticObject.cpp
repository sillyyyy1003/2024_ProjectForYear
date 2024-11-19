#include "StaticObject.h"

#include <memory>

StaticObject::StaticObject()
{
}

void StaticObject::Init(const char* filePath, const char* _objName)
{
	mModel = std::make_shared<PBRModel>();

	mModel->InitWithoutTex(filePath);
	mObjectName = _objName;
}

void StaticObject::LoadTex(PBRConfig::PBRTexList list)
{
	mModel->LoadTex(list);
}

void StaticObject::LoadShaderFile(std::shared_ptr<VertexShader> vs, std::shared_ptr<PixelShader> ps)
{
	mModel->Primitive::LoadDefShader(vs, ps);
}

void StaticObject::LoadShaderFile(const char* vsFile, const char* psFile)
{
	mModel->Primitive::LoadDefShader(vsFile, psFile);
}

void StaticObject::Init(std::shared_ptr<PBRModel> _model, const char* _objName)
{
	mModel = _model;
	mObjectName = _objName;
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
	}
	ImGui::End();
#endif
	mModel->Update(dt);
}

void StaticObject::Draw()
{
	mModel->Draw();
}       

void StaticObject::LoadSaveData(json data, const char* objName)
{
	//Init Model
	//std::string filePath = data[objName]["Filepath"].get<std::string>();
	Vector3 pos = Vector3(data[objName]["Position"][0], data[objName]["Position"][1], data[objName]["Position"][2]);
	mModel->SetPosition(pos);

	//Init Scale
	Vector3 scale = Vector3(data[objName]["Scale"][0], data[objName]["Scale"][1], data[objName]["Scale"][2]);
	mModel->SetScale(scale);

}

void StaticObject::LoadSaveData(json data)
{
	Vector3 pos = Vector3(data[mObjectName.c_str()]["Position"][0], data[mObjectName.c_str()]["Position"][1], data[mObjectName.c_str()]["Position"][2]);
	mModel->SetPosition(pos);

	//Init Scale
	Vector3 scale = Vector3(data[mObjectName.c_str()]["Scale"][0], data[mObjectName.c_str()]["Scale"][1], data[mObjectName.c_str()]["Scale"][2]);
	mModel->SetScale(scale);
}

json StaticObject::SaveData()
{
	json data;
	data["Position"] = { mModel->GetPosition().x,mModel->GetPosition().y,mModel->GetPosition().z };
	data["Scale"] = { mModel->GetScale().x,mModel->GetScale().y,mModel->GetScale().z };

	return data;

}
