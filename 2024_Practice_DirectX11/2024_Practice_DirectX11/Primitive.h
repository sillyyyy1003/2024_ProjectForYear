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
	MULTI,
};

/// <summary>
/// 基本モデル（Cube/Sphere/Cone/Capsule...）
/// </summary>
class Primitive :public Component
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

	/// @brief 頂点データの書き換え用
	std::vector<Vertex::VtxPosNormalTex> mVertices = {};

public:
	Primitive(PrimitiveKind kind);
	virtual ~Primitive() override;

	virtual void Init(const char* filePath = nullptr) = 0;
	virtual void Init(const char* filePath, DirectX::XMFLOAT2) {};
	virtual void Init(const char* filePath, int slices) {};

	virtual void Update(float dt){};
	virtual void Draw(int texSlot = 0) = 0;

	virtual void SetScale(const DirectX::XMFLOAT3& scale) noexcept { mTransform.SetScale(scale); };
	virtual void SetScale(const DirectX::XMFLOAT2& scale) noexcept {};
	virtual void SetScale(float x, float y, float z) noexcept { mTransform.SetScale(x, y, z); };
	virtual void SetScale(const float* scale) noexcept { mTransform.SetScale(scale); };
	virtual void SetScaleXZ(const DirectX::XMFLOAT2& scale) noexcept;

	virtual void SetPosition(const DirectX::XMFLOAT3& position) noexcept { mTransform.SetPosition(position); };
	virtual void SetPosition(float x, float y, float z) noexcept { mTransform.SetPosition(x, y, z); };
	virtual void SetPosition(const float* pos) noexcept { mTransform.SetPosition(pos); };

	virtual const DirectX::XMFLOAT3 GetPosition() noexcept { return mTransform.GetPosition(); };
	virtual const DirectX::XMFLOAT4 GetQuaternion()noexcept { return mTransform.GetQuaternion(); };
	virtual const DirectX::XMFLOAT3 GetRotation()noexcept { return mTransform.GetRotation(); };
	virtual const DirectX::XMFLOAT3 GetScale()noexcept { return mTransform.GetScale(); };

	virtual Material& GetMaterial() noexcept { return mMaterial.material; };

	virtual void SetMaterial(Material _material) noexcept { mMaterial.material = _material; };

	/// @brief テクスチャのファイルパスを取得
	const std::string GetFilePath() noexcept { return mFilePath; };
	void SetFilePath(const char* filePath) noexcept { mFilePath = filePath; };

	/// @brief For Sampler Wrap
	virtual void SetTexUV(DirectX::XMFLOAT2 _texUV)noexcept {};

	virtual	void SetDiffuse(const DirectX::XMFLOAT4& _diffuse) noexcept { mMaterial.material.diffuse = _diffuse; };

	/// @brief Default Shaderを使うかどうか？
	virtual void SetIsDefShader(bool isDefShader)noexcept { this->isDefShader = isDefShader; };

	virtual void SetPixelShader(PixelShader* ps) noexcept { mPS = ps; };
	virtual void SetVertexShader(VertexShader* vs) noexcept { mVS = vs; };

	PixelShader* GetDefPS() const { return mDefPS.get(); };
	VertexShader* GetDefVS() const { return mDefVS.get(); };

	/// @brief Default Shaderの初期化
	virtual void LoadDefShader();
	virtual void LoadDefShader(const char* vsPath,const char* psPath);

	virtual void ResetDefPSShader();
	virtual void ResetDefVSShader();

	virtual void SetDefShader();

	virtual void SetVertices(std::vector<Vertex::VtxPosNormalTex> vertices) noexcept;


private:
	/// @brief デバッグ用　オブジェクト回す
	void UpdateState();

	/// @brief オブジェクトを回転させ　
	/// @param mouseMove 
	/// @param dt 
	void UpdateTransform(DirectX::XMFLOAT2 mouseMove, float dt);
};

