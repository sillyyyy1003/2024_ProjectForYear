#pragma once
#include "PBRModel.h"
#include "Primitive.h"

//This Object can't be interacted only for display
class StaticObject :public Component
{
#ifdef _DEBUG
	float rot[3] = {};
#endif
	std::unique_ptr<Primitive> mModel = nullptr;
	std::string mObjectName;	//オブジェクト名

public:
	StaticObject();
	~StaticObject() override = default;

	/// @brief PBRModelの初期化
	/// @param filePath 3Dモデルのファルパス
	///	@param _objName
	void InitPBR(const char* filePath, const char* _objName);

	/// @brief 一般のモデルの初期化
	/// @param filePath テクスチャのファイルパス
	/// @param _objName 
	/// @param _kind モデルの種類
	///	@param _UVSplit 
	void InitModel(const char* filePath, const char* _objName, PrimitiveConfig::PrimitiveKind _kind, DirectX::XMINT2 _UVSplit = { 1,1 });

	void InitModel(const std::shared_ptr<Texture>& tex, const char* _objName, PrimitiveConfig::PrimitiveKind _kind, DirectX::XMINT2 _UVSplit = { 1,1 });


	void LoadTex(PBRConfig::PBRTexList list);
	void LoadShaderFile(const std::shared_ptr<VertexShader>& vs, const std::shared_ptr<PixelShader>& ps);
	void LoadShaderFile(const char* vsFile, const char* psFile);

	void SetMaterial(const Material& mat) { mModel->SetMaterial(mat); };

	/// @brief 
	/// @param dt delta time
	void Update(float dt);

	void Draw();

	void LoadSaveData(json data);
	json SaveData();

	std::string GetObjectName() { return mObjectName; };

	void SetVertexShader(VertexShader* vs) { mModel->SetVertexShader(vs); };
	void SetPixelShader(PixelShader* ps) { mModel->SetPixelShader(ps); };

	void SwitchToDefShader() { mModel->SwitchToDefShader(); };

	void LoadAlbedoTex(std::shared_ptr<Texture> tex) { mModel->LoadAlbedoTex(tex); };
	void LoadNormalMapTex(std::shared_ptr<Texture> tex) { mModel->LoadNormalMapTex(tex); };
	void LoadMetallicMapTex(std::shared_ptr<Texture> tex) { mModel->LoadMetallicMapTex(tex); };

	const Transform& GetTransform(){ return mModel->mTransform; };

	PixelShader* GetDefPS() { return this->mModel->GetDefPS(); };
	VertexShader* GetDefVS() { return this->mModel->GetDefVS(); };
	

	DirectX::XMFLOAT3 GetPosition() { return mModel->GetPosition(); };
	DirectX::XMFLOAT3 GetScale() { return mModel->GetScale(); };
	DirectX::XMFLOAT3 GetRotation() { return mModel->GetRotation(); };

	void SetPosition(DirectX::XMFLOAT3 pos) { mModel->SetPosition(pos); };
	void SetRotation(DirectX::XMFLOAT3 rot) { mModel->SetRotation(rot); };
	void SetScale(DirectX::XMFLOAT3 scale) { mModel->SetScale(scale); };

};

