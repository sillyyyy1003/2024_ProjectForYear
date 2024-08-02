#pragma once
#include "D3DApp.h"
#include "Shader.h"
#include "Mesh.h"
#include "Transform.h"

/// @brief Box Material含む
class Box3D
{
private:
	//描画関連
	struct MeshData
	{
		std::shared_ptr<Mesh> mesh;
		unsigned int materialID;
	};
	using Meshes = std::vector<MeshData>;
	using Materials = std::vector<Material>;

	/*
	struct Lights
	{
		DirectionLight dirLight;
		PointLight pointLight;
		Spotlight spotLight;
	};
	*/

	//todo:改良：環境光をオブジェクトにする
	struct Light
	{
		DirectX::XMFLOAT4 lightAmbient;
		DirectX::XMFLOAT4 lightDiffuse;
		DirectX::XMFLOAT4 lightDir;
	};

private:
	//VERTEX List
	std::vector<Vertex::VtxPosColorNormal> mVtxData;
	//INDEX list
	std::vector<DWORD> mIdxData;

	Meshes mMeshes; //描画
	std::shared_ptr<Mesh> mBoxMesh;
	std::shared_ptr<VertexShader> mVS = nullptr;
	std::shared_ptr<PixelShader> mPS = nullptr;

	//WVP matrices
	DirectX::XMFLOAT4X4 mWVPMatrix[3] = {};

	//Material　
	Materials mMaterials = {};  //todo Material先设置为一种

	//Light Only Use ambient Light
	//Lights mLights = {};
	Light mLight = {};

	//Camera
	DirectX::XMFLOAT4 mCamera = { 0,0,-5.0,0.0f };


public:

	
	Transform mTransform = {}; //位置・大きさ・回転情報

public:

	Box3D() = default;
	~Box3D() = default;

	void InitResource();

	/// @brief マテリアルを設定
	/// @param _material マテリアル
	/// @param materialID MeshのID
	void SetMaterial(Material _material, int materialID);

	Materials& GetMaterial() { return mMaterials; };

	//Lights& GetLights() { return mLights; };

	/// @brief 描画
	void Draw();

private:
	/// @brief MeshData(頂点)作成
	void CreateMeshes();

	/// @brief Vertex Shader作成
	/// @return 
	void CreateVS();

	/// @brief Pixel Shader作成
	/// @return 
	void CreatePS();

	/// @brief Lightの初期化
	void CreateLights();

	/// @brief 
	void CreateCamera();

	/// @brief Materialを作る
	/// @param matNum materialの数
	void CreateMaterial(int matNum = 6);


	void GenerateMatrix();
	void SetWorldMatrix(const DirectX::XMFLOAT4X4& _world) { mWVPMatrix[0] = _world; }
	void SetViewMatrix(const DirectX::XMFLOAT4X4& _view){ mWVPMatrix[1] = _view; }
	void SetProjMatrix(const DirectX::XMFLOAT4X4& _proj) { mWVPMatrix[2] = _proj; }

	std::vector<DirectX::XMFLOAT3> CalculateVtxNormal(DirectX::XMFLOAT3* pos, DirectX::XMFLOAT3* faceNormals);
};

