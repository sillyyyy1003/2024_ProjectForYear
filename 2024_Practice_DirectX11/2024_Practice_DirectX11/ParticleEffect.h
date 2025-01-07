#pragma once
#include "ParticleMesh.h"
#include "SceneBase.h"
#include "Shader.h"
using namespace DirectX::SimpleMath;

class ParticleEffect:public Component
{
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
		
		//DirectX::XMFLOAT3 velocity;	//���݂̑��x
		//float accumulateTime;		//�o�ߎ���
	};


protected:

	int mParticleNum;	//���q�̐�
	std::unique_ptr<ParticleMesh> mParticleMesh;	//�ʎq��`�悷��

	std::vector<ParticleData> mParticleData;	//�ʎq�̈ʒu���W�Ȃǂ��i��
	DirectX::XMFLOAT3 mEmitPos;
	DirectX::XMFLOAT3 mEmitVel;					//���x�����t��
	DirectX::XMFLOAT3 mEmitAccel;				//�����x
	DirectX::XMFLOAT3 mEmitDirection;			//���˕���
	float time;
	float AliveTime = 2.f;

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
	void InitParticleRenderer(const char* filePath, uint32_t particleNum);

	void InitParticleData(DirectX::XMFLOAT3 emitPos,DirectX::XMFLOAT3 emitAccel,DirectX::XMFLOAT3 direction,float velocity);

	void Update(float dt);
	void Draw();
	void CreateVertexShader(const std::string& filename, const std::string& entrypath);
	void CreateInputLayout(D3D11_INPUT_ELEMENT_DESC* layout, UINT elem_num, const std::string& filename, const std::string& entrypath);
	void CreateConstantBuffer(unsigned int bytesize, ID3D11Buffer** CBuffer);

	std::vector<ParticleMesh::ParticleInstanceData> UpdateInstanceMatrix();
};

