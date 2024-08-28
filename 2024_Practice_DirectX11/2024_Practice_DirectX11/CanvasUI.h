#pragma once
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

	struct MeshBuffer
	{
		std::unique_ptr<Mesh> mesh;
		unsigned int materialID;
	};
	

	//Material & Texture
	MaterialData mMaterial = {};

	//Mesh
	MeshBuffer mMesh = {};

	//VertexShader & PixelShader
	std::unique_ptr<VertexShader> mVS = nullptr;
	std::unique_ptr<PixelShader> mPS = nullptr;


public:
	CanvasUI() = default;
	~CanvasUI() = default;

	/// @brief Create Canvas
	/// @param _fileName �e�N�X�`���t�@�C����
	void InitCanvas(const char* _fileName);

	void Update(float dt);

	void Draw();

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

