#pragma once
#include "Circle.h"
#include "Square.h"
#include "WaterState.h"
#include "FastNoise/FastNoiseLite.h"

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
private:
	bool isResetVertices = false;	//���ׂĂ̒��_�̈ʒu���[���ɂ��邩�ǂ����H
protected:

	WaterStateConfig::WaterStateParam mParam = {};

	//�g��J�n���ǂ����H
	bool isTrigger = true;
	float mNowAmplitude = 0.0f;
	float mWaterTime = 0.0;		//���̔g�̎���
	float mDuration =3.0f;		// �g�̌p������
	WaterStateConfig::WaterBoilingState mBoilingState;//���������邩�ǂ����̔��f

	std::unique_ptr<WaterState> mWaterStates = nullptr;


	//���̕`�� //todo: make the model multi kinds
	std::unique_ptr<Primitive> mModel = nullptr;

	std::string mObjectName;
	bool isApplyParam = false;
	
public:
	
	Water();
	virtual ~Water() = default;

	/// @brief Init object & creating new texture
	/// @param filePath water texture filepath
	/// @param objName �I�u�W�F�N�g��
	///	@param slices ������
	void Init(const char* filePath, const char* objName, int slices = 50);
		
	/// @brief Init Object with existing texture
	/// @param tex ���Ƀ��[�h���ꂽ�e�N�X�`��
	/// @param objName �I�u�W�F�N�g��
	///	@param slices ������
	void Init(const std::shared_ptr<Texture>& tex, const char* objName, int slices = 50);


	void Update(float dt);

	/// @brief �A�b�v�f�[�g�̍Ō�ōs�� �`��̒��_�̍X�V�ƃV�F�[�_�Ƀf�[�^��������
	void RenderUpdate();

	void Draw();

	/// @brief WaterParam�̓��e���V�F�[�_�ɏ�������
	void WriteShader();

	void LoadDefShader(const std::shared_ptr<VertexShader>& vs, const std::shared_ptr<PixelShader>& ps);
	void LoadShader(const std::shared_ptr<VertexShader>& vs, const std::shared_ptr<PixelShader>& ps);


	/// @brief Load Save Data & Init Object Data
	/// @param data json fileData
	/// @param objName dataName
	void LoadSaveData(json data, const char* objName);

	/// @brief �I�u�W�F�N�g�̃f�[�^��json�t�@�C���ɏ�������
	/// @return 
	json SaveData();

	void SetTexture(std::shared_ptr<Texture> tex);

	void RewriteVertices();

	void SetWaterBoilingState(WaterStateConfig::WaterBoilingState _state);
	void SetWaterState(WaterStateConfig::WaterState _state);

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

