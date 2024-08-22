#pragma once
#include "Shader.h"
#include "Transform.h"

class CameraDCC;
/// @brief �n�ʁE���ʁE���ȂǂɎg����
class Plane3D
{
private:
	struct MeshData
	{
		std::shared_ptr<Mesh> mesh = nullptr;
		unsigned int materialID = 0;
	};
	using Meshes = std::vector<MeshData>;
	using Materials = std::vector<Material>;

	std::shared_ptr<VertexShader> mVS = nullptr;
	std::shared_ptr<PixelShader> mPS = nullptr;

	Meshes mMeshes;//�`��p���b�V��
	std::shared_ptr<Texture> mTex = nullptr;//�e�N�X�`��
	Materials mMaterials = {};

	//WVP matrices

public:

	Transform mTransform = {};

public:

	Plane3D() = default;
	~Plane3D() = default;
	
	void InitResource(const char* _fileName);
	void Update(float dt);
	void Draw(int texSlot = 0);

	void SetPixelShader(std::shared_ptr<PixelShader> ps);
	void SetVertexShader(std::shared_ptr<VertexShader> vs);

	/// @brief ���ʂ̃T�C�Y��ݒ肷��
	/// @param width 
	/// @param height 
	const void SetSize(float width, float height);
private:

	const void CreateMeshes();
	const void CreateMaterial(int matNum = 2);
	const void CreateTexture(const char* _fileName);
};

