#pragma once
#include "Collider.h"
#include "SceneBase.h"
#include "Tile.h"

class MissionPaper :public Component
{
private:

protected:

	std::unique_ptr<Tile> mModel = nullptr;//描画する部分
	std::unique_ptr<BoxCollider> mCollider = nullptr; //当たり判定

	bool isClicked = false;
	std::string mObjectName;
	//Trigger used to update collider
	bool isModelStateChange = false;

	int mObjectState = 0;
	// Used for clicked or Hover
	DirectX::XMFLOAT4 mEffect = { 1,1,1,1 };

public:

	MissionPaper();
	~MissionPaper() override = default;

	/// @brief Use existing texture to Init
	/// @param tex existing tex
	///	@param _objName mObjectName
	void Init(std::shared_ptr<Texture> tex,const char* _objName);

	/// @brief Create texture to init
	/// @param filePath texture file path
	/// @param _objName mObjectName
	void Init(const char* filePath,const char* _objName);

	void Update(float dt);

	void Draw();


	void SetPosition(DirectX::XMFLOAT3 pos);
	void SetScale(const DirectX::XMFLOAT2& scale);

	bool GetClicked() { return this->isClicked; };

protected:

	void PreUpdate(float dt);

	void GameUpdate(float dt);

	void LateUpdate(float dt);

	void InitCollider();


	/// @brief 状態変化をListenerを通知する
	void NotifyModelStateChangeListener();
	/// @brief 状態変化をリセットする
	void ClearModelStateChangeListener();
	bool GetModelStateChange() { return isModelStateChange; };


	void OnStateNone();

	void OnStateHover();

	void OnStateClicked();

	void UpdateCollider();
};

