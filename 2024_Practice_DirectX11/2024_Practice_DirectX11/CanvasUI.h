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
	

protected:

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
	std::unique_ptr<VertexShader> mDefVS = nullptr;	// Default Vertex Shader
	std::unique_ptr<PixelShader> mDefPS = nullptr;	// Default Pixel Shader
	VertexShader* mVS = nullptr;
	PixelShader* mPS = nullptr;

	//�ŏ��̑傫�����L�^���Aviewport�ω�����Ƃ��ɑ傫����ω����Ȃ��悤��
	DirectX::XMFLOAT2 mOriginScale = {};
	DirectX::XMFLOAT2 mOriginPos = {};

	bool isDefShader = true;

	std::string mTexResource = {};

public:
	CanvasUI() = default;
	virtual ~CanvasUI();

	/// @brief UI�̃e�N�X�`���ƃV�F�[�_�̏�����
	///	��{UI/No Lighting Effect
	/// @param _fileName �e�N�X�`���t�@�C����
	/// @param size
	/// @param isDefaultSize
	virtual void InitResource(const char* _fileName = nullptr, DirectX::XMFLOAT2 size = {1,1}, bool isDefaultSize = true);

	/// @brief �ʒu�̏�����
	/// @param pos �s�N�Z���ʒu
	virtual void InitData(DirectX::XMFLOAT3 pos);

	virtual void Update(float dt);

	virtual void Draw();

	/// @brief Canvas�̃T�C�Y��ݒ�
	/// @param x ��
	/// @param y ��
	void SetSize(float x, float y);

	/// @brief �������ёւ��`��
	/// @param z 
	void SetPosZ(float z);

	/// @brief �e�N�X�`���̃t�@�C���p�X���擾
	/// @return �t�@�C���p�X
	std::string GetFilePath() const { return mTexResource; };

	/// @brief �ʒu���擾
	/// @return 
	DirectX::XMFLOAT3 GetPos() const { return mTransform.GetPosition(); };

protected:

	/// @brief ���b�V���쐬
	void CreateMeshBuffer();

	/// @brief �e�N�X�`���쐬
	/// @param _fileName �e�N�X�`���t�@�C����
	void CreateTexture(const char* _fileName);

	/// @brief �}�e���A���쐬
	/// @param _fileName �e�N�X�`���t�@�C����
	void CreateMaterial(const char* _fileName);

	virtual void LoadShader();

	/// @brief ui�̃X�P�[�����v�Z���āAui�̑傫����ۂ�
	/// @return viewWidth&viewHeight�̌v�Z
	void UpdateScale();

	void UpdatePos();

	/// @brief �I�u�W�F�N�g��Ԃ��V�F�[�_�ɏ�������
	virtual void WriteShader();

};

