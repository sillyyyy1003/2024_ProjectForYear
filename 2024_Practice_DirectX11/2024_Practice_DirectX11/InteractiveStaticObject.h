#pragma once
#include "Collider.h"
#include "PBRModel.h"
#include "SceneBase.h"

namespace InteractiveConfig
{
	struct RimLightEffect
	{
		DirectX::XMFLOAT3 rimColor = { 1.f,1.f,1.f };
		float rimIntensity = 0.0f;
	};



}

//now only use pbr model
class InteractiveStaticObject :public Component
{
protected:

	// Used for clicked or Hover
	InteractiveConfig::RimLightEffect mEffect;
	DirectX::XMFLOAT3 mColliderExtents = { 0,0,0 };


	std::shared_ptr<Primitive> mModel = nullptr;
	std::unique_ptr<BoxCollider> mCollider = nullptr;
	std::string mObjectName;	//オブジェクト名

	bool isUseRimLight = true;


	//Trigger used to update collider
	bool isModelStateChange = false;

	// object state
	int mObjectState = 0;
	bool isClicked = false;

	//Debug用
#ifdef _DEBUG
	std::unique_ptr<Cube> mDebugColliderMesh = nullptr;
	bool isShowCollider = false;
#endif

	bool isUsePBRModel = true;
	

public:
	InteractiveStaticObject();
	~InteractiveStaticObject() override = default;

	/// @brief 
	/// @param filePath
	///	@param _objName
	virtual void InitPBRModel(const char* filePath,const char* _objName);
	virtual void InitPBRModel(std::shared_ptr<PBRModel> _model, const char* _objName);

	virtual void InitModel(const char* filePath, const char* _objName);
	virtual void InitModel(const std::shared_ptr<Texture>& _file, const char* _objName);

	virtual void Init(PrimitiveConfig::PrimitiveKind _kind, const std::shared_ptr<Texture>& filePath, const char* _objName, DirectX::XMINT2 _UVSplit={1,1});

	virtual void LoadTex(PBRConfig::PBRTexList list);
	virtual void LoadShaderFile(const std::shared_ptr<VertexShader>& vs,const std::shared_ptr<PixelShader>& ps);
	virtual void LoadShaderFile(const char* vsFile, const char* psFile);

	/// @brief 
	/// @param dt delta time
	virtual void Update(float dt);

	virtual void Draw();

	virtual bool GetClicked();

	//====================================
	//	外側でモデルを設定できるのは以下のみ
	//====================================
	void SetModelPosition(DirectX::XMFLOAT3 pos);
	void SetModelRotation(DirectX::XMFLOAT3 rot);
	void SetModelScale(DirectX::XMFLOAT3 scale);

	void SetModelPosition(const float* position);
	void SetModelRotation(const float* rot);
	void SetModelScale(const float* scale);


	json SaveData();
	virtual void LoadSaveData(json data, const char* objName);
	virtual void LoadSaveData(json data);

	DirectX::XMFLOAT3 GetPosition() { return mModel->GetPosition(); };
	DirectX::XMFLOAT3 GetScale() { return mModel->GetScale(); };
	DirectX::XMFLOAT3 GetRotation() { return mModel->GetRotation(); };
	DirectX::XMFLOAT4 GetQuaternion() { return mModel->GetQuaternion(); };

	void SetVertexShader(VertexShader* vs) { mModel->SetVertexShader(vs); };
	void SetPixelShader(PixelShader* ps) { mModel->SetPixelShader(ps); };

	void SwitchToDefShader() { mModel->SwitchToDefShader(); };

	void LoadAlbedoTex(std::shared_ptr<Texture> tex) { mModel->LoadAlbedoTex(tex); };
	void LoadNormalMapTex(std::shared_ptr<Texture> tex) { mModel->LoadNormalMapTex(tex); };
	void LoadMetallicMapTex(std::shared_ptr<Texture> tex) { mModel->LoadMetallicMapTex(tex); };
	const Transform& GetTransform() const { return mModel->mTransform; };
	
	PixelShader* GetDefPS() const { return mModel->GetDefPS(); };

	void UseRimLightEffect();
	void DisableRimLightEffect();

	BoxCollider* GetCollider()const { return mCollider.get(); };
protected:

	void InitCollider();

	void TriggerListener();

	void PreUpdate(float dt);

	void GameUpdate(float dt);

	void LateUpdate(float dt);

	void UpdateCollider();

	/// @brief 状態変化をListenerを通知する
	void NotifyModelStateChangeListener();
	/// @brief 状態変化をリセットする
	void ClearModelStateChangeListener();
	bool GetModelStateChange() { return isModelStateChange; };


	void OnStateNone();

	void OnStateHover();

	void OnStateClicked();
};

