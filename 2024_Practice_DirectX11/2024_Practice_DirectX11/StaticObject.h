﻿#pragma once
#include "PBRModel.h"
#include "Primitive.h"

//This Object can't be interacted only for display
class StaticObject :public Component
{
	std::shared_ptr<PBRModel> mModel = nullptr;
	std::string mObjectName;	//オブジェクト名

public:
	StaticObject();
	~StaticObject() override = default;

	/// @brief 
	/// @param filePath
	///	@param _objName
	void Init(const char* filePath, const char* _objName);
	void LoadTex(PBRConfig::PBRTexList list);
	void LoadShaderFile(std::shared_ptr<VertexShader> vs, std::shared_ptr<PixelShader>ps);
	void LoadShaderFile(const char* vsFile, const char* psFile);

	void Init(std::shared_ptr<PBRModel> _model, const char* _objName);


	/// @brief 
	/// @param dt delta time
	void Update(float dt);

	void Draw();

	void LoadSaveData(json data, const char* objName);
	void LoadSaveData(json data);
	json SaveData();

	std::string GetObjectName() { return mObjectName; };
};
