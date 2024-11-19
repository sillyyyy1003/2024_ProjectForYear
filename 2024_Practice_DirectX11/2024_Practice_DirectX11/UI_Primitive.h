#pragma once
#include "Primitive.h"
#include "SceneBase.h"
#include "Transform.h"
#include "UVAnimation.h"

namespace UIPrimitiveConfig
{
	enum class UI_PrimitiveKind
	{
		SQUARE,
		CIRCLE,
		CAPSULE,
		ROUND_SQUARE,
	};


	constexpr UINT defaultSlices = 16;
}

//For UI Graphics Base class
class UI_Primitive :Component
{
public:
	Transform mTransform;

protected:
	UIPrimitiveConfig::UI_PrimitiveKind mKind;


	//Material & Texture
	MaterialData mMaterial = {};

	//Mesh
	std::unique_ptr<Mesh> mMesh = {};

	//VertexShader & PixelShader
	std::shared_ptr<VertexShader> mDefVS = nullptr;	// Default Vertex Shader
	std::shared_ptr<PixelShader> mDefPS = nullptr;	// Default Pixel Shader
	VertexShader* mVS = nullptr;
	PixelShader* mPS = nullptr;


	//最初の大きさを記録し、viewport変化するときに大きさを変化しないように
	DirectX::XMFLOAT2 mOriginScale = {1.0f,1.0f};
	DirectX::XMFLOAT2 mOriginPos = {};

	bool isDefShader = true;
	bool isUseUVAnimation = false;//UVAnimationを使うか

	std::string mFilePath = {};
	std::string mObjectName = {};

	std::unique_ptr<UVAnimation> mUvAnimation = nullptr;
	std::vector<Vertex::VtxPosNormalTex> mVertices;

	bool isResetVertex = false;

public:
	UI_Primitive(UIPrimitiveConfig::UI_PrimitiveKind kind);

	virtual ~UI_Primitive() override;


	virtual void Init(const char* filePath, int slices = 0, DirectX::XMINT2 _split = { 1,1 }) {};
	virtual void Init(const char* filePath, DirectX::XMFLOAT2 _squareSize, int slices = UIPrimitiveConfig::defaultSlices, DirectX::XMINT2 _split = { 1,1 }) {};

	
	virtual void Init(const std::shared_ptr<Texture>& tex, int slices = 0, DirectX::XMINT2 _split = { 1,1 }) {};
	virtual void Init(const std::shared_ptr<Texture>& tex, DirectX::XMFLOAT2 _squareSize, int slices = UIPrimitiveConfig::defaultSlices, DirectX::XMINT2 _split = { 1,1 }) {};
	
	virtual void CreateMesh(){};
	virtual void CreateMesh(int slices){};

	virtual void CreateMaterial();
	virtual void CreateTexture(const char* filePath);
	virtual void CreateTexture(const std::shared_ptr<Texture>& texture);

	virtual void Update();
	virtual void Draw(int texSlot = 0){};

	virtual void SetPosition(const DirectX::XMFLOAT3& pos) noexcept;

	/// @brief Set UI Size
	virtual void SetScale(float width, float height);
	/// @brief Set UI Size
	virtual void SetScale(const DirectX::XMFLOAT2& size);
	virtual void SetScale(float scale);

	/// @brief 自動並び替え描画
	/// @param z layer depth
	virtual void SetPosZ(float z) noexcept;

	/// @brief Set UI Position
	virtual void SetPosition(float x, float y) noexcept;
	/// @brief Set UI Position
	virtual void SetPosition(const DirectX::XMFLOAT2& pos) noexcept;

	virtual DirectX::XMFLOAT3 GetPosition() { return mTransform.GetPosition(); }
	virtual DirectX::XMFLOAT3 GetScale() { return mTransform.GetScale(); }
	virtual DirectX::XMFLOAT4 GetQuaternion() { return mTransform.GetQuaternion(); };
	/// @brief return UI width/Height
	virtual DirectX::XMFLOAT2 GetUIScale();
	virtual DirectX::XMFLOAT2 GetUIPosition();

	virtual const DirectX::XMFLOAT4& GetDiffuseColor();
	virtual const Material& GetMaterial();

	virtual void SetDiffuseColor(const DirectX::XMFLOAT4& color);
	virtual void SetDiffuseColor(const float* color);
	virtual void SetTexture(const std::shared_ptr<Texture>& texture);

	virtual void SetTransparency(float _transparency);

	virtual void LoadPSShader(const char* psShader);
	virtual void LoadVSShader(const char* vsShader);

	virtual void LoadDefShader(const char* vsShader, const char* psShader);
	virtual void LoadDefShader();

	virtual void SetVertices(const std::vector<Vertex::VtxPosNormalTex>& vertices) noexcept;


	virtual void SetDefShader();

	/// @brief Default Shaderに使われるデータの書き込み
	virtual void WriteDefShader(){};

	UVAnimation* GetAnimation() { return mUvAnimation.get(); };

	void ResetVerticesData();
	void ClearResetVertices();
	bool GetVerticesChange() { return isResetVertex; };
	void SetResetVerticesData(bool isResetVerticesData) { this->isResetVertex = isResetVerticesData; };
protected:

	virtual void UpdateScale();
	virtual void UpdatePos();
};

