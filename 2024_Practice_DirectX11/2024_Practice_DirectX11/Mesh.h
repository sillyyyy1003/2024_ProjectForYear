#pragma once
#include "D3DApp.h"
#include <wrl/client.h>

/// @brief 基底 
class Mesh
{
public:

	struct MeshData
	{
		const void* pVertex; //頂点　異なる型のデータを処理する際に汎用的なポインタ
		UINT vertexSize;
		UINT vertexCount;
		const void* pIndex;
		UINT indexSize;//
		UINT indexCount;
		//https://conscious-maple-e95.notion.site/D3D11_PRIMITIVE_TOPOLOGY-65f1aa0b85284bc4b1f1a2f9a8aa81c5?pvs=74
		D3D11_PRIMITIVE_TOPOLOGY topology;

	};

	MeshData mData = {};

	Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> pIndexBuffer;

public:

	Mesh(const MeshData& _data);
	~Mesh();

	ID3D11Buffer* GetVertexBuffer() const { return pVertexBuffer.Get(); };
	ID3D11Buffer* GetIndexBuffer() const { return pIndexBuffer.Get(); };

	/// @brief 頂点バッファを書き換える
	/// @param pVertex 頂点情報
	HRESULT Write(void* pVertex) const;

	void Draw(int count = 0);
private:

	/// @brief Create VtxPosColorNormal Buffer
	HRESULT CreateVertexBuffer(const void* pVertex, UINT size, UINT vertexCount);

	/// @brief  Create Index Buffer
	HRESULT CreateIndexBuffer(const void* pIndex, UINT size, UINT indexCount);



};