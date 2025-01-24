#pragma once
#include "SceneBase.h"

//todo:暂时不考虑使用几何着色器，而是使用顶点着色器和像素着色器完成渲染
///@brief 粒子の描画を司る
class ParticleMesh :public Mesh
{
protected:

	uint32_t mParticleNum = 0;	//Instance Num
	ComPtr<ID3D11Buffer> mInstanceBuffer;

public:

	struct ParticleInstanceData
	{
		DirectX::XMFLOAT4 Color = { 1.f,1.f,1.f,1.f };
		DirectX::XMFLOAT4X4 WorldMatrix;
	};

public:
	ParticleMesh(const MeshData& data, uint32_t particleNum);
	~ParticleMesh();

	void DrawInstance();

	/// @brief InstanceDataを更新する
	/// @param instanceData 
	/// @return 
	HRESULT WriteInstanceData(void* instanceData) const;
};

