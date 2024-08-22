#pragma once
#include "CameraBase.h"
#include "Shader.h"
#include "Mesh.h"
#include "Texture.h"
#include "Transform.h"

class CameraDCC;

/// @brief Box Material含む
class Box3D
{
private:
	//描画関連
	struct MeshData
	{
		std::shared_ptr<Mesh> mesh;
		unsigned int materialID = 0;
	};

	using Meshes = std::vector<MeshData>;
	using Materials = std::vector<Material>;

private:
	//VERTEX List
	//std::vector<Vertex::VtxPosColorNormal> mVtxData;
	std::vector<Vertex::VtxPosNormalTex> mVtxData;
	//INDEX list
	std::vector<DWORD> mIdxData;

	Meshes mMeshes; //描画
	std::shared_ptr<VertexShader> mVS = nullptr;
	std::shared_ptr<PixelShader> mPS = nullptr;

	//Material　
	Materials mMaterials = {};  //todo Material先设置为一种

	//Texture
	std::shared_ptr<Texture> mTex = nullptr;

	
public:

	Transform mTransform = {}; //位置・大きさ・回転情報

public:

	Box3D() = default;
	~Box3D() = default;

	void InitResource(const char* fileName);

	/// @brief マテリアルを設定
	/// @param _material マテリアル
	/// @param materialID MeshのID
	void SetMaterial(Material _material, int materialID);

	/// @brief 描画
	void Draw(int texSlot = 0);

	void Update();

	void SetPixelShader(std::shared_ptr<PixelShader> ps);
	void SetVertexShader(std::shared_ptr<VertexShader> vs);


	Materials& GetMaterial() { return mMaterials; };

private:
	/// @brief MeshData(頂点)作成
	void CreateMeshes();

	/// @brief Materialを作る
	/// @param matNum materialの数
	void CreateMaterial(int matNum = 6);

	/// @brief ファイルを読み込んでテクスチャを作る
	/// @param fileName ファイル名
	void CreateTexture(const char* fileName);



};

