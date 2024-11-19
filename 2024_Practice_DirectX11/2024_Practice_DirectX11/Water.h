#pragma once
#include "Circle.h"
#include "Square.h"

//Water Default Parameter
namespace WaterDefault
{
	//default water material
	const Material defaultMat =
	{
		Color(0.8f, 0.8f, 0.8f, 1.0f),
		Color(0.2f, 0.3f, 0.6f, 0.3f),
		Color(0.8f, 0.8f, 0.8f, 32.0f),
		Color(0.0f, 0.0f, 0.0f, 1.0f),
	};

	//Default circle slice
	constexpr int defaultSlice = 50;
	//Default circle level
	constexpr int defaultLevel = 50;
}

/// @brief �|�[�V�����̋K��ɂȂ�
class Water
{
protected:

	//���̕����\���Ɋւ���p�����[�^
	struct WaterParam
	{
		DirectX::XMFLOAT3  center;  // �g�̒��S�ʒu
		float nowAmplitude;			// �g�̕�
		float amplitude;

		float frequency;			// �g�̕p�x
		float speed;				// �g�̃X�r�[�h
		float sigma;				// �g�̊g�U�͈�
		float time = 0;					// �o�ߎ���
	};

	WaterParam mParam = {};

	//�g��J�n���ǂ����H
	bool isTrigger = true;
	float mDuration = 3.0f;		// �g�̌p������

	//���̕`�� //todo: make the model multi kinds
	std::unique_ptr<Primitive> mModel = nullptr;

	std::string mObjectName;
public:
	
	Water();
	virtual ~Water() = default;

	/// @brief Init object & creating new texture
	/// @param filePath 
	/// @param objName 
	void Init(const char* filePath,const char* objName);

	/// @brief Init Object with existing texture
	/// @param tex 
	/// @param objName 
	void Init(std::shared_ptr<Texture> tex, const char* objName);


	void Update(float dt);

	/// @brief �A�b�v�f�[�g�̍Ō�ōs�� �`��̒��_�̍X�V�ƃV�F�[�_�Ƀf�[�^��������
	void RenderUpdate();

	void Draw();

	/// @brief WaterParam�̓��e���V�F�[�_�ɏ�������
	void WriteShader();



	/// @brief Load Save Data & Init Object Data
	/// @param data json fileData
	/// @param objName dataName
	void LoadSaveData(json data, const char* objName);

	/// @brief �I�u�W�F�N�g�̃f�[�^��json�t�@�C���ɏ�������
	/// @return 
	json SaveData();

	//Circle* GetModel() { return mModel.get(); };

	void SetTexture(std::shared_ptr<Texture> tex) { mModel->LoadTexture(tex); };

protected:


	/// @brief �g�̒��S�_��ݒ肷��
	/// @param centerPos �g�̒��S�_
	void SetCenter(DirectX::XMFLOAT3 centerPos);

	/// @brief �g�̕p�x��ݒ肷��
	/// @param frequency �g�̕p�x
	void SetFrequency(float frequency);

	/// @brief �g�̃X�r�[�h��ݒ肷��
	/// @param speed �g�̃X�r�[�h
	void SetSpeed(float speed);

	/// @brief �g�̕���ݒ肷��
	/// @param amplitude �g�̕�
	void SetAmplitude(float amplitude);

	/// @brief �g�̌p�����Ԃ�ݒ肷��
	/// @param duration �g�̌p������
	void SetWaveDuration(float duration);

	void ResetMaterial();

	/// @brief �㏈��(Shader�ɏ�������)
	/// @param dt delta Time
	void LateUpdate(float dt);
};

