#pragma once
#include "InteractiveStaticObject.h"
#include "Square.h"

class MissionSamplePaper:public InteractiveStaticObject
{
protected:

	std::unique_ptr<Square> mSampleColor;

public:
	void InitPBRModel(std::shared_ptr<PBRModel> _model, const char* _objName) override;
	void InitModel(const char* filePath, const char* _objName) override;
	void InitModel(const std::shared_ptr<Texture>& _file, const char* _objName) override;
	void Init(PrimitiveConfig::PrimitiveKind _kind, const std::shared_ptr<Texture>& filePath, const char* _objName, DirectX::XMINT2 _UVSplit = { 1,1 }) override;

	void LoadDefShader(const std::shared_ptr<VertexShader>& vs, const std::shared_ptr<PixelShader>& ps) override;
	void Draw() override;
	void Update(float dt) override;


	void LoadSaveData(json data) override;

	void SetSampleColor(DirectX::XMFLOAT4 color);

};



