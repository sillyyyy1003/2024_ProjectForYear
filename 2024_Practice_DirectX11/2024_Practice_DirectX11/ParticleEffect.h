#pragma once
#include "ParticleMesh.h"
#include "SceneBase.h"
#include "Shader.h"
using namespace DirectX::SimpleMath;

class ParticleEffect :public Component
{
protected:

	struct ConstantBuffer
	{
		DirectX::XMFLOAT4X4 view;
		DirectX::XMFLOAT4X4 proj;
	};

	struct ParticleData
	{
		Vector3 initPos;
		Vector3 intiVel;			//�������x�����t��
		Vector3 acceleration;		//�����x
		Vector3 pos;

		Color color;				//���q�F
		float accumulateTime = 0.0f;		//�o�ߎ���
		float lifeTime = 0.0f;		//���q�̎���
	};

	int mParticleNum = 0;	//���q�̐�
	float mParticleSize = 0.f;
	std::unique_ptr<ParticleMesh> mParticleMesh;	//���q��`�悷��

	std::vector<ParticleData> mParticleData;	//���q�̏��Ȃǂ��i��

	float rotation = 0.0f;						//Z���̉�]

	DirectX::XMFLOAT3 mEmitPos;					//�����ʒu
	DirectX::XMFLOAT3 mEmitVel;					//���x�����t��
	DirectX::XMFLOAT3 mEmitAccel;				//�����x
	DirectX::XMFLOAT3 mEmitDirection;			//���˕���

	DirectX::XMFLOAT3 mScale = { 1.f,1.f,1.f };
	

	//�~�`�����̎��Ɏg����
	float mRadius = 0.0f;					
	DirectX::XMFLOAT3 mCenter = {};

	float mTime = 0.0f;						//Timer
	float mParticleLifeTime = 0.0f;			//���q�̎���
	float mEffectAliveTime = 0.0f;			//�G�t�F�N�g�̎���

	bool isLoop = true;	//���̃p�[�e�B�N����Loop���邩�H
	bool isAlive = true;	//�쓮���Ă��邩�H

	//Shader
	ComPtr<ID3D11VertexShader> mVertexShader;
	ComPtr<ID3D11InputLayout> mInputLayout;
	ComPtr<ID3D11Buffer> mBuffer;

public:


	ParticleEffect() = default;
	~ParticleEffect() override = default;

	/// @brief �p�[�e�B�N���`��̏�����
	/// @param filePath VertexShader File
	///	@param particleNum ���q��
	///	@param size ���q�̑傫��
	virtual void InitParticleRenderer(const char* filePath, uint32_t particleNum, float size = 0.01f);

	/// @brief �ʒu���̏�����
	/// @param emitPos �����ʒu
	/// @param emitAccel ���������x
	/// @param velocity ���x(Vector)
	virtual void InitParticleData(DirectX::XMFLOAT3 emitPos, DirectX::XMFLOAT3 emitAccel, DirectX::XMFLOAT3 velocity);

	virtual void InitPointOnCircleParticle(const DirectX::XMFLOAT3& center, float radius, const DirectX::XMFLOAT3& emitAccel, const DirectX::XMFLOAT3& velocity);


	/// @brief �F���̏�����
	/// @param color �F
	virtual void InitParticleColor(Color color = { 1,1,1,1 });

	virtual void InitRandomParticleColor();
	virtual void InitRandomParticleColor(const DirectX::XMFLOAT2& colorRange, const DirectX::XMFLOAT2& valueRange = { 0.7f,1.0f }, const DirectX::XMFLOAT2& saturationRange = { 0.7f,1.0f });

	virtual void InitRandomParticleColor(float color, float range, const DirectX::XMFLOAT2& valueRange = { 0.7f,1.0f }, const DirectX::XMFLOAT2& saturationRange = { 0.7f,1.0f });


	virtual void Update(float dt);
	virtual void Draw();
	void CreateVertexShader(const std::string& filename, const std::string& entryPath);
	void CreateInputLayout(D3D11_INPUT_ELEMENT_DESC* layout, UINT elem_num, const std::string& filename, const std::string& entryPath);
	void CreateConstantBuffer(unsigned int byteSize, ID3D11Buffer** CBuffer);

	/// @brief Virtual Function �F����X�s�[�h���̍X�V 
	virtual void UpdateParticle() {};

	/// @brief Instance�X�V
	/// @param dt deltaTime
	/// @return �X�V���ꂽInstanceVector
	virtual std::vector<ParticleMesh::ParticleInstanceData> UpdateInstanceData(float dt);

	/// @brief ���q�̈ʒu������������
	/// @param data ��̗��q�̏��
	/// @param dt deltaTime
	virtual void UpdateParticleState(ParticleData& data, float dt);

	/// @brief ���q�̐F������������
	/// @param instanceData 
	/// @param data 
	virtual void UpdateParticleColor(ParticleMesh::ParticleInstanceData& instanceData, ParticleData& data);

	/// @brief ���ׂĂ̗��q��������Ԃɖ߂�
	virtual void ResetParticle();

	/// @brief ���q�̑������Ԃ�ݒ肷�� ���q�����̓����_���ɂ���
	void SetParticleAliveTime(float time);

	/// @brief �C�t�F�N�g�̑������Ԃ�ݒ肷��
	void SetEffectAliveTime(float time) { mEffectAliveTime = time; };

	void SetEffectAlive(bool isAlive) { this->isAlive = isAlive; };

	void SetLoop(bool isLoop) { this->isLoop = isLoop; };

	/// @brief ���q�T�C�Y��ݒ肷��
	/// @param scale 
	void SetScale(const DirectX::XMFLOAT3& scale) { this->mScale = scale; };

	void SetParticleLifeTime(float lifeTime);

	int GetParticleNum() const { return mParticleNum; };
	float GetParticleSize() const { return mParticleSize; };
	float GetParticleLife()const { return mParticleLifeTime; };

	DirectX::XMFLOAT3& GetEmitAcceleration() { return mEmitAccel; }
	DirectX::XMFLOAT3& GetEmitVelocity() { return mEmitVel; };
};

