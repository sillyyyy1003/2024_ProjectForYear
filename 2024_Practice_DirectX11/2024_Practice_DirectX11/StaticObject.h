#pragma once
#include "PBRModel.h"
#include "Primitive.h"

//This Object can't be interacted only for display
class StaticObject :public Component
{
#ifdef _DEBUG
	float rot[3] = {};
#endif
protected:

	std::unique_ptr<Primitive> mModel = nullptr;
	std::string mObjectName;	//オブジェクト名
	bool isUsePBR = false;
public:
	StaticObject();
	~StaticObject() override = default;

	/// @brief PBRModelの初期化
	/// @param filePath 3Dモデルのファルパス
	///	@param _objName
	virtual void InitPBR(const char* filePath, const char* _objName);

	/// @brief Primitive/Modelの初期化
	/// @param filePath テクスチャのファイルパス
	/// @param _objName 
	/// @param _kind モデルの種類
	///	@param _UVSplit 
	virtual void InitModel(const char* filePath, const char* _objName, PrimitiveConfig::PrimitiveKind _kind, DirectX::XMINT2 _UVSplit = { 1,1 });

	virtual void InitModel(const char* filePath, const char* _objName, PrimitiveConfig::PrimitiveKind _kind, int slices,DirectX::XMINT2 _UVSplit = { 1,1 });

	/// @brief Primitiveの初期化
	/// @param tex テクスチャのファイルパス
	/// @param _objName 
	/// @param _kind モデルの種類
	///	@param _UVSplit 
	virtual void InitModel(const std::shared_ptr<Texture>& tex, const char* _objName, PrimitiveConfig::PrimitiveKind _kind, DirectX::XMINT2 _UVSplit = { 1,1 });

	virtual void InitModel(const std::shared_ptr<Texture>& tex, const char* _objName, PrimitiveConfig::PrimitiveKind _kind, int slices, DirectX::XMINT2 _UVSplit = { 1,1 });

	/// @brief すべてのテクスチャを読み込む
	/// @param list テクスチャリスト
	void LoadTex(PBRConfig::PBRTexList list);
	void SetMaterial(const Material& mat) { mModel->SetMaterial(mat); };

	/// @brief 
	/// @param dt delta time
	virtual void Update(float dt);

	virtual void Draw();
	virtual void DrawWithNewPos(DirectX::XMFLOAT3 pos);

	virtual void LoadSaveData(json data);
	virtual json SaveData();

	const std::string& GetObjectName() { return mObjectName; };

	virtual void SetVertexShader(VertexShader* vs) { mModel->SetVertexShader(vs); };
	virtual void SetPixelShader(PixelShader* ps) { mModel->SetPixelShader(ps); };

	virtual void SwitchToDefShader() { mModel->SwitchToDefShader(); };

	void LoadAlbedoTex(std::shared_ptr<Texture> tex) { mModel->LoadAlbedoTex(tex); };
	void LoadNormalMapTex(std::shared_ptr<Texture> tex) { mModel->LoadNormalMapTex(tex); };
	void LoadMetallicMapTex(std::shared_ptr<Texture> tex) { mModel->LoadMetallicMapTex(tex); };

	const Transform& GetTransform(){ return mModel->mTransform; };

	PixelShader* GetDefPS() { return this->mModel->GetDefPS(); };
	VertexShader* GetDefVS() { return this->mModel->GetDefVS(); };

	void LoadDefShader(const std::shared_ptr<VertexShader>& vs, const std::shared_ptr<PixelShader>& ps);
	void LoadDefShader(const char* vsFile, const char* psFile);


	DirectX::XMFLOAT3 GetPosition() { return mModel->GetPosition(); };
	DirectX::XMFLOAT3 GetScale() { return mModel->GetScale(); };
	DirectX::XMFLOAT3 GetRotation() { return mModel->GetRotation(); };

	virtual void SetPosition(DirectX::XMFLOAT3 pos) { mModel->SetPosition(pos); };
	virtual void SetRotation(DirectX::XMFLOAT3 rot) { mModel->SetRotation(rot); };
	virtual void SetScale(DirectX::XMFLOAT3 scale) { mModel->SetScale(scale); };
	virtual void SetDiffuseColor(const DirectX::XMFLOAT4& diffuseColor);

	virtual void SetTransparency(float _transparency);
	virtual void LoadTexture(const std::shared_ptr<Texture>& tex);


};

