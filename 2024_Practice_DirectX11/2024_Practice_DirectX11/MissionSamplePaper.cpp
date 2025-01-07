#include "MissionSamplePaper.h"

void MissionSamplePaper::InitPBRModel(std::shared_ptr<PBRModel> _model, const char* _objName)
{
	InteractiveStaticObject::InitPBRModel(_model, _objName);
	mSampleColor = std::make_unique<Square>();
	mSampleColor->Init(nullptr,0);
}

void MissionSamplePaper::InitModel(const char* filePath, const char* _objName)
{
	InteractiveStaticObject::InitModel(filePath, _objName);
	mSampleColor = std::make_unique<Square>();
	mSampleColor->Init(nullptr, 0);
}

void MissionSamplePaper::InitModel(const std::shared_ptr<Texture>& _file, const char* _objName)
{
	InteractiveStaticObject::InitModel(_file, _objName);
	mSampleColor = std::make_unique<Square>();
	mSampleColor->Init(nullptr, 0);
}

void MissionSamplePaper::Init(PrimitiveConfig::PrimitiveKind _kind, const std::shared_ptr<Texture>& filePath,
	const char* _objName, DirectX::XMINT2 _UVSplit)
{
	InteractiveStaticObject::Init(_kind, filePath, _objName, _UVSplit);
	mSampleColor = std::make_unique<Square>();
	mSampleColor->Init(nullptr, 0);
}

void MissionSamplePaper::LoadDefShader(const std::shared_ptr<VertexShader>& vs, const std::shared_ptr<PixelShader>& ps)
{
	InteractiveStaticObject::LoadDefShader(vs, ps);
	mSampleColor->LoadDefShader(vs, ps);
}

void MissionSamplePaper::Draw()
{
	InteractiveStaticObject::Draw();
	mSampleColor->GetDefPS()->WriteShader(2, &mEffect);
	mSampleColor->Draw();
}

void MissionSamplePaper::Update(float dt)
{
	InteractiveStaticObject::Update(dt);
	mSampleColor->Update(dt);
}

void MissionSamplePaper::LoadSaveData(json data)
{
	InteractiveStaticObject::LoadSaveData(data);
	mSampleColor->SetPosition(mModel->GetPosition().x, mModel->GetPosition().y + 0.1f, mModel->GetPosition().z);
	mSampleColor->SetRotationInRadian(mModel->GetRotation());
	mSampleColor->mTransform.SetScale(mModel->GetScale() * 0.5f);
}

void MissionSamplePaper::
SetSampleColor(DirectX::XMFLOAT4 color)
{
	mSampleColor->SetDiffuse(color);
}

