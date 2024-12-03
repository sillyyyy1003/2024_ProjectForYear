#pragma once
#include "Cube.h"
#include "D3DUtil.h"
#include "Texture.h"
#include "Shader.h"
#include "Mesh.h"
#include "Primitive.h"
#include "SceneBase.h"
#include "SceneManager.h"
#include "assimp/Importer.hpp"
#include "assimp/cimport.h"
#include "assimp/scene.h"
#include "assimp/matrix4x4.h"



/// @brief This is for pbr Model
class PBRModel :public Primitive
{

private:

	struct PBRMaterial
	{
		Material material = {};
		std::shared_ptr<Texture> albedoTex = nullptr;
		std::shared_ptr<Texture> metallicTex = nullptr;
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

	bool isUpdateTextureResource = false;
public:

	PBRModel();
	~PBRModel();

	void Init(const char* filePath = nullptr);

	/// @brief テクスチャ別などころで読み込み
	/// @param file モデルと基本のマテリアルのみ
	void InitWithoutTex(const char* file);

	void LoadDefShader();
	void LoadDefShader(const std::shared_ptr<VertexShader>& vsShader, const std::shared_ptr<PixelShader>& psShader);

	void Update(float dt);
	void Draw(int texSlot=0);
	void WriteDefShader();

	void LoadAlbedoTex(std::shared_ptr<Texture> tex);
	void LoadNormalMapTex(std::shared_ptr<Texture> tex);
	void LoadMetallicMapTex(std::shared_ptr<Texture> tex);

	void LoadTex(PBRConfig::PBRTexList list);


protected:

	bool Load(const char* file, bool flip = false, bool simpleMode = false);
	bool LoadWithoutTex(const char* file, bool flip = false, bool simpleMode = false);
private:

	std::unique_ptr<Assimp::Importer> importer = nullptr;
	const aiScene* mScene = nullptr;

private:

	Meshes mMeshes = {};
	Materials mMaterials = {};

};

