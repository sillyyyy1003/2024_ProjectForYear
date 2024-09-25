#pragma once
#include "Collider.h"
#include "Primitive.h"


/// @brief 基本オブジェクト・当たり判定付き
///	For Test
class Object
{
protected:

	POINT mOldPos = {};

	int mState = 0;//オブジェクトの状態を司る
	bool isStateChange = false;//状態変更判定

public:
	
	std::unique_ptr<Primitive> 	mModel = nullptr;		//描画
	std::unique_ptr<Collider> 	mCollider = nullptr;	//当たり判定


	Object();
	virtual ~Object();

	/// @brief オブジェクト実態の初期化
	/// @param kind オブジェクトの形状
	virtual void Init(PrimitiveKind kind);

	/// @brief 設定データからオブジェクトを初期化
	/// @param kind オブジェクトの形状
	/// @param filePath テクスチャファイルパス
	virtual void Init(PrimitiveKind kind, const char* filePath);

	/// @brief モデルの初期化(Mesh/Transform..)
	/// @param filePath テクスチャファイルパス
	virtual void InitModel(const char* filePath);

	/// @brief Load Save Data & Init Object Data
	/// @param data json fileData
	/// @param objName dataName
	virtual void LoadSaveData(json data, const char* objName);


	virtual void Update(float dt);
	virtual void Draw();

	DirectX::XMFLOAT3 GetPosition() const { return mModel->GetPosition(); };
	DirectX::XMFLOAT3 GetScale() const { return mModel->GetScale(); };
	DirectX::XMFLOAT4 GetQuaternion() const { return mModel->GetQuaternion(); };
	DirectX::XMFLOAT3 GetRotation()const { return mModel->GetRotation(); }

	std::string GetFilePath() const { return mModel->GetFilePath(); };
	const Material& GetMaterial() { return mModel->GetMaterial(); };

	void SetScale(const DirectX::XMFLOAT3& scale) { mModel->SetScale(scale); };
	void SetScale(const DirectX::XMFLOAT2& scale) {};
	void SetScale(const float* scale) { mModel->mTransform.SetScale(scale); };

	void SetPosition(const DirectX::XMFLOAT3 position) { mModel->mTransform.SetPosition(position); };
	void SetPosition(float x, float y, float z) { mModel->mTransform.SetPosition(x, y, z); };
	void SetPosition(const float* position) { mModel->mTransform.SetPosition(position); };

	void SetMaterial(const Material& mat);

	json SaveData();

	void ResetPSShader();

protected:

	/// @brief 入力・物理演算などの処理を行うフラッグの更新
	/// @param dt Delta Time
	virtual void PreUpdate(float dt);

	/// @brief 入力などに応じて、ゲームロジックの実現
	/// @param dt Delta Time
	virtual void GameUpdate(float dt);

	/// @brief 
	/// @param dt Delta Time
	virtual void LateUpdate(float dt);

	/// @brief Drag状態処理
	/// @param dt Delta Time
	virtual void OnStateDrag(float dt);

	/// @brief Selected状態処理
	/// @param dt Delta Time
	virtual void OnStateSelected(float dt);

};

