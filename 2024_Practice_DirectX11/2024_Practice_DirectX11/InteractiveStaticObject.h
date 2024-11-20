#pragma once
#include "Collider.h"
#include "PBRModel.h"
#include "SceneBase.h"

//now only use pbr model
class InteractiveStaticObject :public Component
{
private:
	struct RimLightEffect
	{
		DirectX::XMFLOAT3 rimColor = { 1.f,1.f,1.f };
		float rimIntensity = 0.0f;
	};
protected:
	
	//std::shared_ptr<PBRModel> mPBRModel = nullptr;
	std::shared_ptr<Primitive> mPBRModel = nullptr;
	std::unique_ptr<BoxCollider> mCollider = nullptr;
	std::string mObjectName;	//オブジェクト名

	//Trigger used to update collider
	bool isModelStateChange = false;

	// object state
	int mObjectState = 0;

	// Used for clicked or Hover
	RimLightEffect mEffect;

	bool isClicked = false;

	//Debug用
	std::unique_ptr<Cube> mDebugColliderMesh = nullptr;
	bool isShowCollider = false;



public:
	InteractiveStaticObject();
	~InteractiveStaticObject() = default;

	/// @brief 
	/// @param filePath
	///	@param _objName
	void Init(const char* filePath,const char* _objName);
	void Init(std::shared_ptr<PBRModel> _model, const char* _objName);
	void LoadTex(PBRConfig::PBRTexList list);
	void LoadShaderFile(const std::shared_ptr<VertexShader>& vs,const std::shared_ptr<PixelShader>& ps);
	void LoadShaderFile(const char* vsFile, const char* psFile);

	/// @brief 
	/// @param dt delta time
	void Update(float dt);

	void Draw();

	bool GetClicked() { return isClicked; };

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
	void LoadSaveData(json data, const char* objName);

	DirectX::XMFLOAT3 GetPosition() { return mPBRModel->GetPosition(); };
	DirectX::XMFLOAT3 GetScale() { return mPBRModel->GetScale(); };
	DirectX::XMFLOAT3 GetRotation() { return mPBRModel->GetRotation(); };
	DirectX::XMFLOAT4 GetQuaternion() { return mPBRModel->GetQuaternion(); };

	void SetVertexShader(VertexShader* vs) { mPBRModel->SetVertexShader(vs); };
	void SetPixelShader(PixelShader* ps) { mPBRModel->SetPixelShader(ps); };

	void SwitchToDefShader() { mPBRModel->SwitchToDefShader(); };

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

