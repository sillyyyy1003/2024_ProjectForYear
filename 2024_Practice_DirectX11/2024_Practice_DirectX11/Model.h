#pragma once
#include "D3DUtil.h"
#include "Transform.h"

#include "assimp/Importer.hpp"
#include "assimp/cimport.h"
#include "assimp/scene.h"
#include "assimp/matrix4x4.h"


class PixelShader;
class VertexShader;
class Texture;
class Model
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

	struct MaterialData
	{
		Material material;
		std::shared_ptr<Texture> tex;
	};

	using Materials = std::vector<MaterialData>;

	struct MeshBuffer
	{
		std::shared_ptr<Mesh> mesh;
		unsigned int materialID;
	};
	using Meshes = std::vector<MeshBuffer>;


public:
	Model();
	~Model();

	void SetVertexShader(std::shared_ptr<VertexShader> vs);
	void SetPixelShader(std::shared_ptr<PixelShader> ps);

	bool Load(const char* file, bool flip = false, bool simpleMode = false);
	void Draw(int texSlot = 0);

private:

	std::shared_ptr<Assimp::Importer> importer = nullptr;
	const aiScene* mScene = nullptr;

private:

	Meshes mMeshes;
	Materials mMaterials;
	std::shared_ptr<VertexShader> mVS = nullptr;
	std::shared_ptr<PixelShader> mPS = nullptr;


};

