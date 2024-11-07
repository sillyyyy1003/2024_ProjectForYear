#pragma once
#include "D3DUtil.h"
#include "Texture.h"
#include "Shader.h"
#include "Mesh.h"
#include "Primitive.h"
#include "SceneBase.h"
#include "assimp/Importer.hpp"
#include "assimp/cimport.h"
#include "assimp/scene.h"
#include "assimp/matrix4x4.h"


/// @brief This is for pbr Model
class PBRModel :public Primitive
{
public:

	Transform mTransform = {};

private:

	struct VertexWithBone
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT2 uv;
		int		boneIndex[4];
		float	boneWeight[4];
		int		boneCount = 0;
	};

	struct PBRMaterial
	{
		Material material = {};
		std::shared_ptr<Texture> albedoTex = nullptr;
		std::shared_ptr<Texture> metallicTex = nullptr;
		std::shared_ptr<Texture> aoTex = nullptr;
		std::shared_ptr<Texture> normalMap = nullptr;
	};

	///Using two texture
	using Materials = std::vector<PBRMaterial>;

	struct MeshBuffer
	{
		std::shared_ptr<Mesh> mesh = nullptr;
		unsigned int materialID;
	};
	using Meshes = std::vector<MeshBuffer>;


public:
	PBRModel();
	~PBRModel();

	void Init(const char* filePath = nullptr);
	
	bool Load(const char* file, bool flip = false, bool simpleMode = false);

	void LoadDefShader() override;

	void Update(float dt);
	void Draw(int texSlot=0) override;
	void WriteDefShader();

	void LoadAlbedoTex(Texture* tex);
	void LoadNormalMapTex(Texture* tex);
	void LoadMetallicMapTex(Texture* tex);
	void LoadAOTex(Texture* tex);
private:

	std::unique_ptr<Assimp::Importer> importer = nullptr;
	const aiScene* mScene = nullptr;

private:

	Meshes mMeshes = {};
	Materials mMaterials = {};
	PBRMaterial mPBRMaterial = {};


};

