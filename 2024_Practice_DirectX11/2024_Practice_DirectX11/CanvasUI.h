#pragma once
#include "Primitive.h"
#include "Transform.h"
#include "UVAnimation.h"

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
	//Material & Texture
	MaterialData mMaterial = {};

	//Mesh
	std::unique_ptr<Mesh> mMesh = {};

	//VertexShader & PixelShader
	std::shared_ptr<VertexShader> mDefVS = nullptr;	// Default Vertex Shader
	std::shared_ptr<PixelShader> mDefPS = nullptr;	// Default Pixel Shader
	VertexShader* mVS = nullptr;
	PixelShader* mPS = nullptr;

	//�ŏ��̑傫�����L�^���Aviewport�ω�����Ƃ��ɑ傫����ω����Ȃ��悤��
	DirectX::XMFLOAT2 mOriginScale = {};
	DirectX::XMFLOAT2 mOriginPos = {};

	bool isDefShader = true;
	bool isUseUVAnimation = false;//UVAnimation���g����

	std::string mFilePath= {};
	std::string mObjectName = {};

	std::unique_ptr<UVAnimation> mUvAnimation = nullptr;
public:
	CanvasUI() = default;
	virtual ~CanvasUI();

	/// @brief UI�̃e�N�X�`���ƃV�F�[�_�̏�����
	///	��{UI/No Lighting Effect
	/// @param _fileName �e�N�X�`���t�@�C����
	///	@param _split ����
	virtual void Init(const char* _fileName = nullptr,DirectX::XMINT2 _split={1,1});

	virtual void Init(MaterialData& _materialData, DirectX::XMINT2 _split = { 1,1 });

	virtual void Init(std::shared_ptr<Texture> tex, DirectX::XMINT2 _split = { 1,1 });

	/// @brief �ʒu�̏�����
	/// @param pos �s�N�Z���ʒu
	virtual void SetPosition(DirectX::XMFLOAT3 pos);

	/// @brief Load Save Data & Init Object Data
	/// @param data json fileData
	/// @param objName dataName
	virtual void LoadSaveData(json data, const char* objName);

	virtual void Update(float dt);
	virtual void Update();


	virtual void Draw();

	/// @brief Canvas�̃T�C�Y��ݒ�
	/// @param width ��
	/// @param height ��
	void SetCanvasSize(float width, float height);

	/// @brief �������ёւ��`��
	/// @param z 
	void SetPosZ(float z);


	/// @brief Set Canvas position
	/// @param x 
	/// @param y 
	void SetPosition(float x, float y);


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

	UVAnimation* GetUVAnimation() { return mUvAnimation.get(); };

	const DirectX::XMFLOAT4& GetDiffuseColor() { return mMaterial.material.diffuse; };

	/// @brief �F�ݒ�
	/// @param color 
	void SetColor(DirectX::XMFLOAT4 color);

	/// @brief �����x�ݒ�
	/// @param _transparency 
	void SetTransparency(float _transparency);

	void LoadPSShader(const char* filePath);
protected:

	/// @brief ���b�V���쐬
	void CreateMeshBuffer();

	/// @brief �e�N�X�`���쐬
	/// @param _fileName �e�N�X�`���t�@�C����
	void CreateTexture(const char* _fileName);

	/// @brief �e�N�X�`���쐬
	/// @param pTex �쐬����Ă���Texture pointer
	void CreateTexture(const std::shared_ptr<Texture>& pTex);

	/// @brief �}�e���A���쐬
	/// @param _fileName �e�N�X�`���t�@�C����
	void CreateMaterial(const char* _fileName);

	/// @brief �}�e���A���쐬
	/// @param _materialData �쐬����Ă���}�e���A���f�[�^
	void CreateMaterial(MaterialData& _materialData);

	void CreateMaterial(const std::shared_ptr<Texture>& tex);

	/// @brief Load Default Shader
	virtual void LoadShader();



	/// @brief �V�F�[�_�t�@�C�������Z�b�g
	/// @param PSFile PS file path
	/// @param VSFile VS file path
	virtual void SetShader(const char* PSFile, const char* VSFile);

	/// @brief ui�̃X�P�[�����v�Z���āAui�̑傫����ۂ�
	/// @return viewWidth&viewHeight�̌v�Z
	void UpdateScale();

	void UpdatePos();

	/// @brief �I�u�W�F�N�g��Ԃ��V�F�[�_�ɏ�������
	virtual void WriteShader();

};

