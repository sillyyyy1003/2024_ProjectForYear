#pragma once
#include "Collider.h"
#include "Primitive.h"
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

	//Material & Texture
	MaterialData mMaterial = {};

	//Mesh
	std::unique_ptr<Mesh> mMesh = {};

	//VertexShader & PixelShader
	std::unique_ptr<VertexShader> mVS = nullptr;
	std::unique_ptr<PixelShader> mPS = nullptr;

	//�}���X�����蔻��p
	std::unique_ptr<BoxCollider> mCollider = nullptr;

	//�ŏ��̑傫�����L�^���Aviewport�ω�����Ƃ��ɑ傫����ω����Ȃ��悤��
	DirectX::XMFLOAT2 mOriginScale = {};
	

public:
	CanvasUI() = default;
	~CanvasUI() = default;

	/// @brief Create Canvas
	/// @param _fileName �e�N�X�`���t�@�C����
	void Init(const char* _fileName=nullptr);

	void Update(float dt);

	void Draw();

	/// @brief Canvas�̃T�C�Y��ݒ�
	/// @param x ��
	/// @param y ��
	void SetSize(float x, float y);

private:
	/// @brief ���b�V���쐬
	void CreateMeshBuffer();

	/// @brief �e�N�X�`���쐬
	/// @param _fileName �e�N�X�`���t�@�C����
	void CreateTexture(const char* _fileName);

	/// @brief �}�e���A���쐬
	/// @param _fileName �e�N�X�`���t�@�C����
	void CreateMaterial(const char* _fileName);

	/// @brief ui�̃X�P�[�����v�Z���āAui�̑傫����ۂ�
	/// @return viewWidth&viewHeight�̌v�Z
	void UpdateScale();

	void UpdatePos();
};

