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

	std::string mFilePath= {};
	std::string mObjectName = {};
public:
	CanvasUI() = default;
	virtual ~CanvasUI();

	/// @brief UI�̃e�N�X�`���ƃV�F�[�_�̏�����
	///	��{UI/No Lighting Effect
	/// @param _fileName �e�N�X�`���t�@�C����
	virtual void Init(const char* _fileName = nullptr);

	/// @brief �ʒu�̏�����
	/// @param pos �s�N�Z���ʒu
	virtual void InitPosition(DirectX::XMFLOAT3 pos);

	/// @brief Load Save Data & Init Object Data
	/// @param data json fileData
	/// @param objName dataName
	virtual void LoadSaveData(json data, const char* objName);

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
	std::string GetFilePath() const { return mFilePath; };

	std::string GetObjectName() const { return mObjectName; };

	/// @brief �ʒu���擾
	/// @return 
	DirectX::XMFLOAT3 GetPosition() const { return mTransform.GetPosition(); };

	/// @brief �傫�����擾
	/// @return X:���@Y:����
	DirectX::XMFLOAT2 GetScaleXY() const { return DirectX::XMFLOAT2{ mTransform.GetPosition().x,mTransform.GetPosition().y }; };

	DirectX::XMFLOAT3 GetScale()const { return mTransform.GetScale(); };
	DirectX::XMFLOAT4 GetQuaternion()const { return mTransform.GetQuaternion(); };
	DirectX::XMFLOAT3 GetRotation()const { return mTransform.GetRotation(); };

	const Material& GetMaterial() { return mMaterial.material; };

	virtual json SaveData(const char* objName);

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

