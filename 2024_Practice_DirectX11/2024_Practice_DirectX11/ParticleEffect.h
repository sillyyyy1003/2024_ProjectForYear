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
		Vector3 intiVel;			//初期速度方向付き
		Vector3 acceleration;		//加速度
		Vector3 pos;
		
		//DirectX::XMFLOAT3 velocity;	//現在の速度
		//float accumulateTime;		//経過時間
	};


protected:

	int mParticleNum;	//粒子の数
	std::unique_ptr<ParticleMesh> mParticleMesh;	//量子を描画する

	std::vector<ParticleData> mParticleData;	//量子の位置座標などを司る
	DirectX::XMFLOAT3 mEmitPos;
	DirectX::XMFLOAT3 mEmitVel;					//速度方向付き
	DirectX::XMFLOAT3 mEmitAccel;				//加速度
	DirectX::XMFLOAT3 mEmitDirection;			//発射方向
	float time;
	float AliveTime = 2.f;

	//Shader
	ComPtr<ID3D11VertexShader> mVertexShader;
	ComPtr<ID3D11InputLayout> mInputLayout;
	ComPtr<ID3D11Buffer> mBuffer;

public:


	ParticleEffect() = default;
	~ParticleEffect() override = default;

	/// @brief パーティクル描画の初期化
	/// @param filePath VertexShader File
	///	@param particleNum 粒子数
	void InitParticleRenderer(const char* filePath, uint32_t particleNum);

	void InitParticleData(DirectX::XMFLOAT3 emitPos,DirectX::XMFLOAT3 emitAccel,DirectX::XMFLOAT3 direction,float velocity);

	void Update(float dt);
	void Draw();
	void CreateVertexShader(const std::string& filename, const std::string& entrypath);
	void CreateInputLayout(D3D11_INPUT_ELEMENT_DESC* layout, UINT elem_num, const std::string& filename, const std::string& entrypath);
	void CreateConstantBuffer(unsigned int bytesize, ID3D11Buffer** CBuffer);

	std::vector<ParticleMesh::ParticleInstanceData> UpdateInstanceMatrix();
};

