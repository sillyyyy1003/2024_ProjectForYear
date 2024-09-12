#pragma once
#include "SceneBase.h"
#include "Transform.h"
#include "Shader.h"

/// <summary>
/// メッシュの種類
/// </summary>
enum PrimitiveKind
{
	PRIMITIVE_DEFAULT,
	CUBE,
	CAPSULE,
	SPHERE,
	CYLINDER,
	PLANE,
};

/// <summary>
/// 基本モデル（Cube/Sphere/Cone/Capsule...）
/// </summary>
class Primitive:public Component
{
	//POINT mOldPos = {};
	//int mState = 0;//Debug用
protected:
	struct MaterialData
	{
		Material material = {};
		std::unique_ptr<Texture> tex = nullptr;
	};

	VertexShader* mVS = nullptr;
	PixelShader* mPS = nullptr;

	std::shared_ptr<VertexShader> mDefVS = nullptr;
	std::shared_ptr<PixelShader> mDefPS = nullptr;

	bool isDefShader = true;

	PrimitiveKind mKind = PRIMITIVE_DEFAULT;
	MaterialData mMaterial = {};

public:

	/// @brief 大きさ・位置・回転などの情報
	Transform mTransform = {};


public:
	Primitive(PrimitiveKind kind);
	virtual ~Primitive();

	virtual void Init(const char* filePath = nullptr) = 0;
	virtual void Update(float dt);
	virtual void Draw(int texSlot = 0) = 0;

	virtual void SetScale(const DirectX::XMFLOAT3& scale) { mTransform.SetScale(scale); };
	virtual void SetScale(const DirectX::XMFLOAT2& scale) {};

	virtual DirectX::XMFLOAT3 GetPosition() const { return mTransform.GetPosition(); };
	virtual DirectX::XMFLOAT4 GetQuaternion() const { return mTransform.GetRotationQuat(); };
	virtual DirectX::XMFLOAT3 GetRotation() const { return mTransform.GetRotation(); };
	virtual DirectX::XMFLOAT3 GetScale() const { return mTransform.GetScale(); };

	
	virtual void SetTexUV(DirectX::XMFLOAT2 _texUV) {};

	virtual	void SetDiffuse(DirectX::XMFLOAT4 _diffuse) { mMaterial.material.diffuse = _diffuse; };
	virtual void SetMaterial(Material _material) { mMaterial.material = _material; };

	/// @brief Default Shaderを使うかどうか？
	virtual void SetDefShader(bool isDefShader) { this->isDefShader = isDefShader; };
	virtual void SetPixelShader(PixelShader* ps) { mPS = ps; };
	virtual void SetVertexShader(VertexShader* vs) { mVS = vs; };

private:
	/// @brief デバッグ用　オブジェクト回す
	void UpdateState();

	/// @brief オブジェクトを回転させ　
	/// @param mouseMove 
	/// @param dt 
	void UpdateTransform(DirectX::XMFLOAT2 mouseMove, float dt);
};

