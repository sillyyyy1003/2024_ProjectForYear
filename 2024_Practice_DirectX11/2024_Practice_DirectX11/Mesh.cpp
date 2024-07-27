#include "Mesh.h"
#include "DebugLog.h"
#include "DXTrace.h"

Mesh::Mesh(const MeshData& _data)
{
	HR(CreateVertexBuffer(_data.pVertex, _data.vertexSize, _data.vertexCount));

    //Index ����Ȃ�
    if (_data.pIndex)
    {
		HR(CreateIndexBuffer(_data.pIndex, _data.indexSize, _data.indexCount));
    }

    mData = _data;


    rsize_t vertexTotalSize = static_cast<size_t>(_data.vertexSize) * _data.vertexCount;
    void* pVertex = new char[vertexTotalSize];
    memcpy_s(pVertex, vertexTotalSize, _data.pVertex, vertexTotalSize);
    mData.pVertex = pVertex;


    rsize_t indexTotalSize = static_cast<size_t>(_data.indexSize) * _data.indexCount;
    void* pIndex = new char[indexTotalSize];
    memcpy_s(pIndex, indexTotalSize, _data.pIndex, indexTotalSize);
    mData.pIndex = pIndex;
}

HRESULT Mesh::CreateVertexBuffer(const void* pVertex, UINT size, UINT vertexCount)
{
	//--- �쐬����o�b�t�@�̏��
	D3D11_BUFFER_DESC bufDesc = {};
	bufDesc.ByteWidth = size * vertexCount;
	bufDesc.Usage = D3D11_USAGE_DEFAULT;
	bufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	/*if (isWrite)
	{
		bufDesc.Usage = D3D11_USAGE_DYNAMIC;
		bufDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}*/

	//--- �o�b�t�@�̏����l��ݒ�
	D3D11_SUBRESOURCE_DATA subResource = {};
	subResource.pSysMem = pVertex;

	//--- ���_�o�b�t�@�̍쐬
	HRESULT hr;
	ID3D11Device* pDevice = gD3D->GetDevice();
	hr = pDevice->CreateBuffer(&bufDesc, &subResource, pVertexBuffer.GetAddressOf());

	if (FAILED(hr))
	{
		DebugLog::LogError("Failed to Create VertexBuffer");
	}
	return hr;
}

HRESULT Mesh::CreateIndexBuffer(const void* pIndex, UINT size, UINT indexCount)
{
	// �C���f�b�N�X�T�C�Y�̊m�F
	switch (size)
	{
	default:
		return E_FAIL;
	case 2:
	case 4:
		break;
	}

	// �o�b�t�@�̏���ݒ�
	D3D11_BUFFER_DESC bufDesc = {};
	bufDesc.ByteWidth = size * indexCount;
	bufDesc.Usage = D3D11_USAGE_DEFAULT;
	bufDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	// �o�b�t�@�̏����f�[�^
	D3D11_SUBRESOURCE_DATA subResource = {};
	subResource.pSysMem = pIndex;

	// �C���f�b�N�X�o�b�t�@����
	ID3D11Device* pDevice = gD3D->GetDevice();
	HRESULT hr;
	hr = pDevice->CreateBuffer(&bufDesc, &subResource, pIndexBuffer.GetAddressOf());
	if (FAILED(hr))
	{
		DebugLog::LogError("Failed to Create VertexBuffer");
	}
	return hr;
}

HRESULT Mesh::Write(void* pVertex) const
{
	//if (!mData.isWrite) { return E_FAIL; }

	HRESULT hr;
	ID3D11Device* pDevice = gD3D->GetDevice();
	ID3D11DeviceContext* pContext = gD3D->GetContext();
	D3D11_MAPPED_SUBRESOURCE mapResource;

	// �f�[�^�R�s�[
	hr = pContext->Map(pVertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapResource);
	if (SUCCEEDED(hr))
	{
		rsize_t size = mData.vertexCount * mData.vertexSize;
		memcpy_s(mapResource.pData, size, pVertex, size);
		pContext->Unmap(pVertexBuffer.Get(), 0);
	}
	return hr;
}

void Mesh::Draw(int count)
{
	ID3D11DeviceContext* pContext = gD3D->GetContext();
	UINT stride = mData.vertexSize;
	UINT offset = 0;

	// �g�|���W�̐ݒ�
	ID3D11HullShader* hullShader;
	pContext->HSGetShader(&hullShader, nullptr, nullptr);
	if (hullShader)
		hullShader->Release();
	else
		pContext->IASetPrimitiveTopology(mData.topology);

	// ���_�o�b�t�@�ݒ�
	pContext->IASetVertexBuffers(0, 1, pVertexBuffer.GetAddressOf(), &stride, &offset);

	// �`��
	if (mData.indexCount > 0)
	{
		DXGI_FORMAT format;
		switch (mData.indexSize)
		{
		case 4: format = DXGI_FORMAT_R32_UINT; break;
		case 2: format = DXGI_FORMAT_R16_UINT; break;
		}
		pContext->IASetIndexBuffer(pIndexBuffer.Get(), format, 0);
		pContext->DrawIndexed(count ? count : mData.indexCount, 0, 0);
	}
	else
	{
		// ���_�o�b�t�@�݂̂ŕ`��
		pContext->Draw(count ? count : mData.vertexCount, 0);
	}

}
