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
		Vector3 intiVel;			//初期速度方向付き
		Vector3 acceleration;		//加速度
		Vector3 pos;

		Color color;				//粒子色
		float accumulateTime = 0.0f;		//経過時間
		float lifeTime = 0.0f;		//粒子の寿命
	};

	int mParticleNum = 0;	//粒子の数
	float mParticleSize = 0.f;
	std::unique_ptr<ParticleMesh> mParticleMesh;	//粒子を描画する

	std::vector<ParticleData> mParticleData;	//粒子の情報などを司る

	float rotation = 0.0f;						//Z軸の回転

	DirectX::XMFLOAT3 mEmitPos;					//発生位置
	DirectX::XMFLOAT3 mEmitVel;					//速度方向付き
	DirectX::XMFLOAT3 mEmitAccel;				//加速度
	DirectX::XMFLOAT3 mEmitDirection;			//発射方向

	DirectX::XMFLOAT3 mScale = { 1.f,1.f,1.f };
	

	//円形生成の時に使われる
	float mRadius = 0.0f;					
	DirectX::XMFLOAT3 mCenter = {};

	float mTime = 0.0f;						//Timer
	float mParticleLifeTime = 0.0f;			//粒子の寿命
	float mEffectAliveTime = 0.0f;			//エフェクトの寿命

	bool isLoop = true;	//このパーティクルはLoopするか？
	bool isAlive = true;	//作動しているか？

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
	///	@param size 粒子の大きさ
	virtual void InitParticleRenderer(const char* filePath, uint32_t particleNum, float size = 0.01f);

	/// @brief 位置情報の初期化
	/// @param emitPos 発生位置
	/// @param emitAccel 発生加速度
	/// @param velocity 速度(Vector)
	virtual void InitParticleData(DirectX::XMFLOAT3 emitPos, DirectX::XMFLOAT3 emitAccel, DirectX::XMFLOAT3 velocity);

	virtual void InitPointOnCircleParticle(const DirectX::XMFLOAT3& center, float radius, const DirectX::XMFLOAT3& emitAccel, const DirectX::XMFLOAT3& velocity);


	/// @brief 色情報の初期化
	/// @param color 色
	virtual void InitParticleColor(Color color = { 1,1,1,1 });

	virtual void InitRandomParticleColor();
	virtual void InitRandomParticleColor(const DirectX::XMFLOAT2& colorRange, const DirectX::XMFLOAT2& valueRange = { 0.7f,1.0f }, const DirectX::XMFLOAT2& saturationRange = { 0.7f,1.0f });

	virtual void InitRandomParticleColor(float color, float range, const DirectX::XMFLOAT2& valueRange = { 0.7f,1.0f }, const DirectX::XMFLOAT2& saturationRange = { 0.7f,1.0f });


	virtual void Update(float dt);
	virtual void Draw();
	void CreateVertexShader(const std::string& filename, const std::string& entryPath);
	void CreateInputLayout(D3D11_INPUT_ELEMENT_DESC* layout, UINT elem_num, const std::string& filename, const std::string& entryPath);
	void CreateConstantBuffer(unsigned int byteSize, ID3D11Buffer** CBuffer);

	/// @brief Virtual Function 色情報やスピード情報の更新 
	virtual void UpdateParticle() {};

	/// @brief Instance更新
	/// @param dt deltaTime
	/// @return 更新されたInstanceVector
	virtual std::vector<ParticleMesh::ParticleInstanceData> UpdateInstanceData(float dt);

	/// @brief 粒子の位置情報を書き込み
	/// @param data 一つの粒子の情報
	/// @param dt deltaTime
	virtual void UpdateParticleState(ParticleData& data, float dt);

	/// @brief 粒子の色情報を書き込み
	/// @param instanceData 
	/// @param data 
	virtual void UpdateParticleColor(ParticleMesh::ParticleInstanceData& instanceData, ParticleData& data);

	/// @brief すべての粒子を初期状態に戻す
	virtual void ResetParticle();

	/// @brief 粒子の存続時間を設定する 粒子寿命はランダムにする
	void SetParticleAliveTime(float time);

	/// @brief イフェクトの存続時間を設定する
	void SetEffectAliveTime(float time) { mEffectAliveTime = time; };

	void SetEffectAlive(bool isAlive) { this->isAlive = isAlive; };

	void SetLoop(bool isLoop) { this->isLoop = isLoop; };

	/// @brief 粒子サイズを設定する
	/// @param scale 
	void SetScale(const DirectX::XMFLOAT3& scale) { this->mScale = scale; };

	void SetParticleLifeTime(float lifeTime);

	int GetParticleNum() const { return mParticleNum; };
	float GetParticleSize() const { return mParticleSize; };
	float GetParticleLife()const { return mParticleLifeTime; };

	DirectX::XMFLOAT3& GetEmitAcceleration() { return mEmitAccel; }
	DirectX::XMFLOAT3& GetEmitVelocity() { return mEmitVel; };
};

