#pragma once
#include "Collider.h"
#include "Primitive.h"
#include "StaticObject.h"


/// Potionを構成する素材
class Ingredient :public StaticObject
{
private:
	///todo:以下は使えない
	float mBasicPrice = 100.0f;
	float mCurrPrice = 100.0f;
	float mFreshness = 100.0f;		//新鮮度
	float mBasicDecayRate = 0.01f;
	int	mIngredientState = 0;		//素材の状態
	float mShelfLife = 100.0f;		//保存時間

	POINT mOldPos = {};//Old MouseCursor Pos
	int mState = 0;//オブジェクトの状態を司る
	bool isStateChange = false;//状態変更判定
	bool isUsed = false;


	struct RimLightEffect
	{
		DirectX::XMFLOAT3 rimColor = { 1.f,1.f,1.f };
		float rimIntensity = 0.0f;
	};

	// Used for clicked or Hover
	RimLightEffect mEffect;
	DirectX::XMFLOAT3 mColliderExtents = { 0,0,0 };

	//Debug用
#ifdef _DEBUG
	std::unique_ptr<Cube> mDebugColliderMesh = nullptr;
	bool isShowCollider = false;
#endif
protected:

	std::string mIngredientName = "";
	std::unique_ptr<BoxCollider> mCollider;

public:
	Ingredient();
	~Ingredient() = default;

	void Init(const char* filePath, const char* _objName, PrimitiveConfig::PrimitiveKind _kind, DirectX::XMINT2 _UVSplit);

	/// @brief 金額を設定する
	void SetPrice(float _price);

	/// @brief 名前を設定する
	void SetName(const char* _ingredientName);

	void Update(float dt) override;
	void Draw() override;

	void UpdatePrice();

	/// @brief 素材の情報
	/// @return format: Name:xxx Freshness:xxx Price:xxx
	std::string GetIngredientInfo();

	const DirectX::XMFLOAT4& GetColor() { return mModel->GetMaterial().diffuse; };
	void SetColor(const DirectX::XMFLOAT4& color){mModel->SetDiffuse(color);}

	void SetPosition(DirectX::XMFLOAT3 pos);
	void SetScale(DirectX::XMFLOAT3 scale);


protected:
	void InitCollider();

	void PreUpdate(float dt);
	void GameUpdate(float dt);
	void LateUpdate(float dt);

	virtual void OnStateNone(float dt);
	virtual void OnStateDrag(float dt);
	virtual void OnStateSelected(float dt);

	void UpdateCollider();
};