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
	SQUARE,
	CIRCLE,
	MULTI,
};

struct MaterialData
{
	Material material = {};
	std::shared_ptr<Texture> tex = nullptr;
};


namespace PBRConfig
{
	enum class PBRTex
	{
		ALBEDO,
		METALLIC,
		NORMAL,
		//AO,
	};

	using PBRTexList = std::unordered_map<PBRTex, std::shared_ptr<Texture>>;
}


/// <summary>
/// 基本モデル（Cube/Sphere/Cone/Capsule...）
/// </summary>
class Primitive :public Component
{

protected:

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
	std::vector<Vertex::VtxPosNormalTex> mVertices;
	std::vector<DWORD> mIndices;

public:
	Primitive(PrimitiveKind kind);
	virtual ~Primitive() override;

	virtual void CreateMesh(UINT levels, UINT slices) {};
	virtual void CreateMesh(UINT slices) {};


	virtual void Init(const char* filePath = nullptr) {};
	virtual void Init(const char* filePath, int slices) {};
	virtual void Init(const char* filePath, int slices, int levels) {};
	virtual void Init(const std::shared_ptr<Texture>& tex) {};
	virtual void Init(const std::shared_ptr<Texture>& tex, int slices) {};
	virtual void Init(const std::shared_ptr<Texture>& tex, int slices, int levels) {};




	virtual void CreateMaterial();
	virtual void CreateMaterial(Material& material) { mMaterial.material = material; };
	virtual void CreateTexture(const char* filePath);

	virtual void Update(float dt) {};
	virtual void Draw(int texSlot = 0) = 0;

	virtual void SetScale(const DirectX::XMFLOAT3& scale) noexcept { mTransform.SetScale(scale); };
	virtual void SetScale(const DirectX::XMFLOAT2& scale) noexcept {};
	virtual void SetScale(float x, float y, float z) noexcept { mTransform.SetScale(x, y, z); };
	virtual void SetScale(const float* scale) noexcept { mTransform.SetScale(scale); };
	virtual void SetScaleXZ(const DirectX::XMFLOAT2& scale) noexcept;
	virtual void SetScaleXY(const DirectX::XMFLOAT2& scale) noexcept;

	virtual void SetPosition(const DirectX::XMFLOAT3& position) noexcept { mTransform.SetPosition(position); };
	virtual void SetPosition(float x, float y, float z) noexcept { mTransform.SetPosition(x, y, z); };
	virtual void SetPosition(const float* pos) noexcept { mTransform.SetPosition(pos); };


	virtual void SetRotation(const float* rot)noexcept { mTransform.SetRotationInDegree(rot[0], rot[1], rot[2]); };
	virtual void SetRotation(const DirectX::XMFLOAT3& rotation)noexcept { mTransform.SetRotationInDegree(rotation); };
	virtual void SetRotation(float x, float y, float z)noexcept { mTransform.SetRotationInDegree(x, y, z); };


	virtual const DirectX::XMFLOAT3 GetPosition() noexcept { return mTransform.GetPosition(); };
	virtual const DirectX::XMFLOAT4 GetQuaternion()noexcept { return mTransform.GetQuaternion(); };
	virtual const DirectX::XMFLOAT3 GetRotation()noexcept { return mTransform.GetRotation(); };
	virtual const DirectX::XMFLOAT3 GetScale()noexcept { return mTransform.GetScale(); };

	virtual Material& GetMaterial() noexcept { return mMaterial.material; };

	virtual void SetMaterial(Material _material) noexcept { mMaterial.material = _material; };
	virtual void SetAmbient(const DirectX::XMFLOAT4& _ambient) noexcept { mMaterial.material.ambient = _ambient; };
	virtual	void SetDiffuse(const DirectX::XMFLOAT4& _diffuse) noexcept { mMaterial.material.diffuse = _diffuse; };
	virtual void LoadTexture(const std::shared_ptr<Texture>& tex);

	/// @brief テクスチャのファイルパスを取得
	const std::string GetFilePath() noexcept { return mFilePath; };
	void SetFilePath(const char* filePath) noexcept { mFilePath = filePath; };

	/// @brief For Sampler Wrap
	virtual void SetTexUV(DirectX::XMFLOAT2 _texUV) noexcept {};

	virtual void SetPixelShader(PixelShader* ps) noexcept;
	virtual void SetVertexShader(VertexShader* vs) noexcept;

	PixelShader* GetDefPS() const { return mDefPS.get(); };
	VertexShader* GetDefVS() const { return mDefVS.get(); };

	PixelShader* GetPS() const { return mPS; };
	VertexShader* GetVS() const { return mVS; };

	/// @brief Default Shaderの初期化
	virtual void LoadDefShader();
	virtual void LoadDefShader(const char* vsPath, const char* psPath);
	virtual void LoadDefShader(const std::shared_ptr<VertexShader>& vsShader,const std::shared_ptr<PixelShader>& psShader);

	virtual void SwitchToDefShader();

	virtual void SetDefShader();
	/// @brief mVS/mPSに書き込む
	virtual void WriteShader() {};

	/// @brief Def VS/PSに書き込む
	virtual void WriteDefShader() {};
	virtual void WritePixelShader() {};
	virtual void WriteVertexShader() {};

	virtual void SetVertices(std::vector<Vertex::VtxPosNormalTex> vertices) noexcept;
	virtual void SetIndices(std::vector<DWORD> indices) noexcept;

	virtual std::shared_ptr<Mesh> GetMesh() { return nullptr; };


	//=====================================
	// Used in PBR Model
	//=====================================
	virtual void LoadAlbedoTex(std::shared_ptr<Texture> tex) {};
	virtual void LoadNormalMapTex(std::shared_ptr<Texture> tex) {};
	virtual void LoadMetallicMapTex(std::shared_ptr<Texture> tex) {};
	virtual void InitWithoutTex(const char* file){};
	
	virtual void LoadTex(PBRConfig::PBRTexList list) {};


};

