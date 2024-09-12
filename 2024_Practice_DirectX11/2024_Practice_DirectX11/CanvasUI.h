#pragma once
#include "Collider.h"
#include "Primitive.h"
#include "Transform.h"

class PixelShader;
class VertexShader;
class Texture;

/// <summary>
/// Using D3D Render 
/// </summary>
class CanvasUI
{
public:

	Transform mTransform = {};


private:

	struct MaterialData
	{
		Material material;
		std::unique_ptr<Texture> tex;
	};

	//Material & Texture
	MaterialData mMaterial = {};

	//Mesh
	std::unique_ptr<Mesh> mMesh = {};

	//VertexShader & PixelShader
	std::unique_ptr<VertexShader> mVS = nullptr;
	std::unique_ptr<PixelShader> mPS = nullptr;

	//マルス当たり判定用
	std::unique_ptr<BoxCollider> mCollider = nullptr;

	//最初の大きさを記録し、viewport変化するときに大きさを変化しないように
	DirectX::XMFLOAT2 mOriginScale = {};
	

public:
	CanvasUI() = default;
	~CanvasUI() = default;

	/// @brief Create Canvas
	/// @param _fileName テクスチャファイル名
	void Init(const char* _fileName=nullptr);

	void Update(float dt);

	void Draw();

	/// @brief Canvasのサイズを設定
	/// @param x 幅
	/// @param y 高
	void SetSize(float x, float y);

private:
	/// @brief メッシュ作成
	void CreateMeshBuffer();

	/// @brief テクスチャ作成
	/// @param _fileName テクスチャファイル名
	void CreateTexture(const char* _fileName);

	/// @brief マテリアル作成
	/// @param _fileName テクスチャファイル名
	void CreateMaterial(const char* _fileName);

	/// @brief uiのスケールを計算して、uiの大きさを保つ
	/// @return viewWidth&viewHeightの計算
	void UpdateScale();

	void UpdatePos();
};

