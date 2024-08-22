#pragma once
#include "Transform.h"

class PixelShader;
class VertexShader;
class Texture;
/// @brief �{�^���ȂǂɎg����
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
	/// @param _fileName �e�N�X�`���t�@�C����
	void InitCanvas(const char* _fileName);



private:
	/// @brief ���b�V���쐬
	void CreateMeshBuffer();

	/// @brief �e�N�X�`���쐬
	/// @param _fileName �e�N�X�`���t�@�C����
	void CreateTexture(const char* _fileName);

	/// @brief �}�e���A���쐬
	/// @param _fileName �e�N�X�`���t�@�C����
	void CreateMaterial(const char* _fileName);



};

