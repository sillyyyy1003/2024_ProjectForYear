#pragma once
#include "Collider.h"
#include "Mission.h"
#include "SceneBase.h"
#include "Square.h"

class MissionPaper :public Component
{
private:

	struct RimLightEffect
	{
		DirectX::XMFLOAT3 rimColor = { 1.f,1.f,1.f };
		float rimIntensity = 0.0f;
	};

	// Used for clicked or Hover
	RimLightEffect mEffect;
protected:

	std::unique_ptr<Square> mPaper = nullptr;//�`�悷�镔��
	std::unique_ptr<BoxCollider> mCollider = nullptr; //�����蔻��
	std::unique_ptr<Mission> mMission;


	bool isClicked = false;
	std::string mObjectName;
	//Trigger used to update collider
	bool isModelStateChange = false;

	int mObjectState = 0;

public:

	MissionPaper();
	~MissionPaper() override = default;

	/// @brief Use existing texture to Init
	/// @param tex existing tex
	///	@param _objName mObjectName
	void Init(const std::shared_ptr<Texture>& tex, const char* _objName);

	/// @brief Create texture to init
	/// @param filePath texture file path
	/// @param _objName mObjectName
	void Init(const char* filePath, const char* _objName);

	void InitMission(float reward, DirectX::XMFLOAT4 targetColor);

	void Update(float dt);

	void Draw();


	void SetPosition(DirectX::XMFLOAT3 pos);
	void SetScale(const DirectX::XMFLOAT2& scale);

	bool GetClicked();

	void LoadDefShader(const std::shared_ptr<VertexShader>& pVS,const std::shared_ptr<PixelShader>& mPS);

	
protected:

	void PreUpdate(float dt);

	void GameUpdate(float dt);

	void LateUpdate(float dt);

	void InitCollider();


	/// @brief ��ԕω���Listener��ʒm����
	void NotifyModelStateChangeListener();
	/// @brief ��ԕω������Z�b�g����
	void ClearModelStateChangeListener();
	bool GetModelStateChange() { return isModelStateChange; };


	void OnStateNone();

	void OnStateHover();

	void OnStateClicked();

	void UpdateCollider();
};