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

class Model :public Primitive
{
public:

	//Transform mTransform = {};

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

	struct MaterialData
	{
		Material material = {};
		std::shared_ptr<Texture> tex = nullptr;
	};

	using Materials = std::vector<MaterialData>;

	struct MeshBuffer
	{
		std::shared_ptr<Mesh> mesh = nullptr;
		unsigned int materialID;
	};
	using Meshes = std::vector<MeshBuffer>;


public:
	Model();
	~Model();

	void Init(const char* filePath = nullptr);
	bool Load(const char* file, bool flip = false, bool simpleMode = false);

	void Update(float dt);
	void Draw(int texSlot = 0);

	void WriteDefShader();

private:

	std::unique_ptr<Assimp::Importer> importer = nullptr;
	const aiScene* mScene = nullptr;

private:

	Meshes mMeshes = {};
	Materials mMaterials = {};

};

