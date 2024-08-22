#pragma once
#include "Transform.h"

class PixelShader;
class VertexShader;
class Texture;
/// @brief ボタンなどに使われる
class CanvasUI
{
public:

	Transform mTransform = {};

private:

	struct MaterialData
	{
		Material material;
		std::shared_ptr<Texture> tex;
	};

	struct MeshBuffer
	{
		std::shared_ptr<Mesh> mesh;
		unsigned int materialID;
	};
	

	//Material & Texture
	MaterialData mMaterial = {};

	//Mesh
	MeshBuffer mMeshes = {};

	//VertexShader & PixelShader
	std::shared_ptr<VertexShader> mVS = nullptr;
	std::shared_ptr<PixelShader> mPS = nullptr;


public:
	CanvasUI() = default;
	~CanvasUI() = default;

	/// @brief Create Canvas
	/// @param _fileName テクスチャファイル名
	void InitCanvas(const char* _fileName);



private:
	/// @brief メッシュ作成
	void CreateMeshBuffer();

	/// @brief テクスチャ作成
	/// @param _fileName テクスチャファイル名
	void CreateTexture(const char* _fileName);

	/// @brief マテリアル作成
	/// @param _fileName テクスチャファイル名
	void CreateMaterial(const char* _fileName);



};

