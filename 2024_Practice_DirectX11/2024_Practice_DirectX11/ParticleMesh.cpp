#include "ParticleMesh.h"
#include "GameApp.h"

ParticleMesh::ParticleMesh(const MeshData& data, uint32_t particleNum) :Mesh(data), mParticleNum(particleNum)
{
	//--- 作成するバッファの情報
	D3D11_BUFFER_DESC bufDesc = {};
	bufDesc.Usage = D3D11_USAGE_DEFAULT;
	bufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	if (mData.isWrite)
	{
		bufDesc.Usage = D3D11_USAGE_DYNAMIC;
		bufDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}
	bufDesc.ByteWidth = sizeof(ParticleInstanceData) * particleNum;

	ID3D11Device* pDevice = GameApp::Get()->GetDevice();
	HR(pDevice->CreateBuffer(&bufDesc, nullptr, mInstanceBuffer.GetAddressOf()));
}

ParticleMesh::~ParticleMesh()
{
}

void ParticleMesh::DrawInstance()
{
    ID3D11DeviceContext* context = gD3D->GetContext();

    UINT strides[2] = { mData.vertexSize, sizeof(ParticleInstanceData) };
    UINT offsets[2] = { 0, 0 };
    ID3D11Buffer* buffers[2] = { pVertexBuffer.Get(), mInstanceBuffer.Get() };


    // 頂点バッファとInstanceBuffer設定
    context->IASetVertexBuffers(0, 2, buffers, strides, offsets);

    // トポロジの設定
    context->IASetPrimitiveTopology(mData.topology);

    if (mData.indexCount > 0)
    {
        DXGI_FORMAT format = (mData.indexSize == 4) ? DXGI_FORMAT_R32_UINT : DXGI_FORMAT_R16_UINT;
        context->IASetIndexBuffer(pIndexBuffer.Get(), format, 0);
        context->DrawIndexedInstanced(mData.indexCount, mParticleNum, 0, 0, 0);
    }
    else
    {
        context->DrawInstanced(mData.vertexCount, mParticleNum, 0, 0);
    }
}

HRESULT ParticleMesh::WriteInstanceData(void* instanceData) const
{
    HRESULT hr;
    ID3D11Device* pDevice = GameApp::Get()->GetDevice();
    ID3D11DeviceContext* pContext = gD3D->GetContext();
    D3D11_MAPPED_SUBRESOURCE mapResource;
    // データコピー
    hr = pContext->Map(mInstanceBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapResource);
    if (SUCCEEDED(hr))
    {
        rsize_t size = static_cast<size_t>(mParticleNum) * sizeof(ParticleInstanceData);
        memcpy_s(mapResource.pData, size, instanceData, size);
        pContext->Unmap(mInstanceBuffer.Get(), 0);
    }
    return hr;
}
