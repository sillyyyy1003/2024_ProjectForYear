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
	CYLINDER_ONECAP,
	PLANE,
	CIRCLE,
	
};

/// <summary>
/// 基本モデル（Cube/Sphere/Cone/Capsule...）
/// </summary>
class Primitive:public Component
{

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

	std::string mFilePath;

public:

	/// @brief 大きさ・位置・回転などの情報
	Transform mTransform = {};


public:
	Primitive(PrimitiveKind kind);
	virtual ~Primitive() override;

	virtual void Init(const char* filePath = nullptr) = 0;
	virtual void Init(const char* filePath, DirectX::XMFLOAT2) {};
	virtual void Init(const char* filePath, int slices) {};

	virtual void Update(float dt);
	virtual void Draw(int texSlot = 0) = 0;

	virtual void SetScale(const DirectX::XMFLOAT3& scale) { mTransform.SetScale(scale); };
	virtual void SetScale(const DirectX::XMFLOAT2& scale) {};
	virtual void SetScale(float x, float y, float z) { mTransform.SetScale(x, y, z); };
	virtual void SetScale(const float* scale) { mTransform.SetScale(scale); };
	virtual void SetScaleXZ(const DirectX::XMFLOAT2& scale);

	virtual void SetPosition(const DirectX::XMFLOAT3 position) { mTransform.SetPosition(position); };
	virtual void SetPosition(float x, float y, float z) { mTransform.SetPosition(x, y, z); };

	virtual DirectX::XMFLOAT3 GetPosition() const { return mTransform.GetPosition(); };
	virtual DirectX::XMFLOAT4 GetQuaternion() const { return mTransform.GetQuaternion(); };
	virtual DirectX::XMFLOAT3 GetRotation() const { return mTransform.GetRotation(); };
	virtual DirectX::XMFLOAT3 GetScale() const { return mTransform.GetScale(); };

	virtual Material& GetMaterial() { return mMaterial.material; };
	virtual void SetMaterial(Material _material) { mMaterial.material = _material; };

	/// @brief テクスチャのファイルパスを取得
	std::string GetFilePath() { return mFilePath; };
	void SetFilePath(const char* filePath) { mFilePath = filePath; };

	/// @brief For Sampler Wrap
	virtual void SetTexUV(DirectX::XMFLOAT2 _texUV) {};

	virtual	void SetDiffuse(DirectX::XMFLOAT4 _diffuse) { mMaterial.material.diffuse = _diffuse; };


	/// @brief Default Shaderを使うかどうか？
	virtual void SetIsDefShader(bool isDefShader) { this->isDefShader = isDefShader; };

	virtual void SetPixelShader(PixelShader* ps) { mPS = ps; };
	virtual void SetVertexShader(VertexShader* vs) { mVS = vs; };

	PixelShader* GetDefPS() const { return mDefPS.get(); };
	VertexShader* GetDefVS() const { return mDefVS.get(); };

	/// @brief Default Shaderの初期化
	virtual void LoadDefShader();
	virtual void LoadDefShader(const char* vsPath,const char* psPath);

private:
	/// @brief デバッグ用　オブジェクト回す
	void UpdateState();

	/// @brief オブジェクトを回転させ　
	/// @param mouseMove 
	/// @param dt 
	void UpdateTransform(DirectX::XMFLOAT2 mouseMove, float dt);
};

