#include "Shader.h"
#include <cstdio>
#include "DebugLog.h"
#include "D3DUtil.h"
#include "GameApp.h"
#pragma comment(lib, "d3dcompiler.lib")

Shader::Shader(ShaderKind kind) :mShader(kind)
{
	
}

Shader::~Shader()
{

}


HRESULT Shader::CompileShader(const char* pCode)
{
	static const char* pTargetList[] =
	{
		"vs_5_0",
		"ps_5_0",
		"gs_5_0",
		//"hs_5_0",
		//"ds_5_0",
	};

	HRESULT hr;
	ID3DBlob* pBlob;
	ID3DBlob* error;
	UINT compileFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
	hr = D3DCompile(pCode, strlen(pCode), nullptr, nullptr, nullptr,
		"main", pTargetList[mShader], compileFlag, 0, &pBlob, &error);
	if (FAILED(hr)) { return hr; }

	// シェーダ作成
	hr = Create(pBlob->GetBufferPointer(), (UINT)pBlob->GetBufferSize());
	SAFE_RELEASE(pBlob);
	SAFE_RELEASE(error);
	return hr;
}

HRESULT Shader::LoadShaderFile(const char* _fileName)
{
	HRESULT hr = E_FAIL;

	//Fileを読み込み
	FILE* fp;
	fopen_s(&fp, _fileName, "rb");
	if (!fp)
	{
		DebugLog::LogError("Read Error {}", _fileName);
		return hr;
	}

	// ファイルのサイズを調べる
	int fileSize = 0;
	fseek(fp, 0, SEEK_END);
	fileSize = ftell(fp);

	// メモリに読み込み
	fseek(fp, 0, SEEK_SET);
	char* pData = new char[fileSize];
	fread(pData, fileSize, 1, fp);
	fclose(fp);

	//pData->Shader字节码
	hr = Create(pData, fileSize);
	
	if (pData) { delete[] pData; }
	return hr;
}

void Shader::WriteShader(UINT slot, void* pData)
{
	//大きさを超えないよう
	if (slot < mBuffers.size())
	{
		gD3D->GetContext()->UpdateSubresource(mBuffers[slot].Get(), 0, nullptr, pData, 0, 0);
	}
}


void Shader::SetTexture(UINT slot, Texture* _texture)
{
	if (slot >= mTextures.size()) { return; }
	ID3D11ShaderResourceView* pTex = _texture ? _texture->GetResource() : nullptr;
	mTextures[slot] = pTex;
	switch (mShader)
	{
	case ShaderKind::Vertex:
		gD3D->GetContext()->VSSetShaderResources(slot, 1, &pTex);
		break;
	case ShaderKind::Pixel:
		gD3D->GetContext()->PSSetShaderResources(slot, 1, &pTex);
		break;
	case ShaderKind::Hull:
		gD3D->GetContext()->HSSetShaderResources(slot, 1, &pTex);
		break;
	case ShaderKind::Domain:
		gD3D->GetContext()->DSSetShaderResources(slot, 1, &pTex);
		break;
		default:
			return;
	}
}


HRESULT Shader::Create(void* pData, UINT size)
{
	HRESULT hr;
	ID3D11Device* pDevice = gD3D->GetDevice();

	// 解析用のリフレクション作成
	ID3D11ShaderReflection* pReflection;
	hr = D3DReflect(pData, size, IID_PPV_ARGS(&pReflection));
	if (FAILED(hr)) { return hr; }

	// 定数バッファ作成
	D3D11_SHADER_DESC shaderDesc;
	pReflection->GetDesc(&shaderDesc);
	//mBufferを調整、シェーダによるコンストバッファを全部読み込み、大きさを調整
	//shaderDesc.ConstantBuffers:ConstBufferの数
	//mBuffers.resize(shaderDesc.ConstantBuffers, nullptr);
	mBuffers.resize(shaderDesc.ConstantBuffers);

	for (UINT i = 0; i < shaderDesc.ConstantBuffers; ++i)
	{
		// シェーダーの定数バッファの情報を取得
		D3D11_SHADER_BUFFER_DESC shaderBufDesc;
		ID3D11ShaderReflectionConstantBuffer* cbuf = pReflection->GetConstantBufferByIndex(i);
		cbuf->GetDesc(&shaderBufDesc);

		// 作成するバッファの情報
		D3D11_BUFFER_DESC bufDesc = {};
		bufDesc.ByteWidth = shaderBufDesc.Size;
		bufDesc.Usage = D3D11_USAGE_DEFAULT;
		bufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

		// バッファの作成
		hr = pDevice->CreateBuffer(&bufDesc, nullptr, mBuffers[i].GetAddressOf());
		if (FAILED(hr)) { return hr; }
	}

	// テクスチャ領域作成
	mTextures.resize(shaderDesc.BoundResources, nullptr);
	
	//実際のシェーダの作成
	hr = CreateShader(pData, size);
	return hr;

}

VertexShader::VertexShader():
Shader(ShaderKind::Vertex),
mVertexShader(nullptr),
mInputLayout(nullptr)
{

}

VertexShader::~VertexShader()
{

}

HRESULT VertexShader::CreateShader(void* pData, UINT size)
{
	HRESULT hr;
	ID3D11Device* pDevice = gD3D->GetDevice();

	// シェーダー作成
	hr = pDevice->CreateVertexShader(pData, size, nullptr, mVertexShader.GetAddressOf());
	if (FAILED(hr)) { return hr; }

	//InputLayout作成
	/*
	シェーダ作成時にシェーダリフレクションを通してインプットレイアウトを取得
	セマンティクスの配置などから識別子を作成
	識別子が登録済→再利用、なければ新規作成
	https://blog.techlab-xe.net/dxc-shader-reflection/
	*/
	ID3D11ShaderReflection* pReflection;
	D3D11_SHADER_DESC shaderDesc;
	D3D11_INPUT_ELEMENT_DESC* pInputDesc;
	D3D11_SIGNATURE_PARAMETER_DESC sigDesc;

	DXGI_FORMAT formats[][4] =
	{
		{
			DXGI_FORMAT_R32_UINT,
			DXGI_FORMAT_R32G32_UINT,
			DXGI_FORMAT_R32G32B32_UINT,
			DXGI_FORMAT_R32G32B32A32_UINT,
		}, {
			DXGI_FORMAT_R32_SINT,
			DXGI_FORMAT_R32G32_SINT,
			DXGI_FORMAT_R32G32B32_SINT,
			DXGI_FORMAT_R32G32B32A32_SINT,
		}, {
			DXGI_FORMAT_R32_FLOAT,
			DXGI_FORMAT_R32G32_FLOAT,
			DXGI_FORMAT_R32G32B32_FLOAT,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
		},
	};

	hr = D3DReflect(pData, size, IID_PPV_ARGS(&pReflection));
	if (FAILED(hr)) { return hr; }

	pReflection->GetDesc(&shaderDesc);
	pInputDesc = new D3D11_INPUT_ELEMENT_DESC[shaderDesc.InputParameters];
	for (UINT i = 0; i < shaderDesc.InputParameters; ++i)
	{
		pReflection->GetInputParameterDesc(i, &sigDesc);
		pInputDesc[i].SemanticName = sigDesc.SemanticName;
		pInputDesc[i].SemanticIndex = sigDesc.SemanticIndex;

		// http://marupeke296.com/TIPS_No17_Bit.html
		BYTE elementCount = sigDesc.Mask;
		elementCount = (elementCount & 0x05) + ((elementCount >> 1) & 0x05);
		elementCount = (elementCount & 0x03) + ((elementCount >> 2) & 0x03);

		switch (sigDesc.ComponentType)
		{
		case D3D_REGISTER_COMPONENT_UINT32:
			pInputDesc[i].Format = formats[0][elementCount - 1];
			break;
		case D3D_REGISTER_COMPONENT_SINT32:
			pInputDesc[i].Format = formats[1][elementCount - 1];
			break;
		case D3D_REGISTER_COMPONENT_FLOAT32:
			pInputDesc[i].Format = formats[2][elementCount - 1];
			break;
		}
		pInputDesc[i].InputSlot = 0;
		pInputDesc[i].AlignedByteOffset = i == 0 ? 0 : D3D11_APPEND_ALIGNED_ELEMENT;
		pInputDesc[i].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		pInputDesc[i].InstanceDataStepRate = 0;
	}

	hr = pDevice->CreateInputLayout(
		pInputDesc, shaderDesc.InputParameters,
		pData, size, mInputLayout.GetAddressOf()
	);

	delete[] pInputDesc;
	return hr;
}


void VertexShader::SetShader()
{
	ID3D11DeviceContext* pContext = gD3D->GetContext();

	pContext->VSSetShader(mVertexShader.Get(), nullptr, 0);
	pContext->IASetInputLayout(mInputLayout.Get());

	for (int i = 0; i < mBuffers.size(); ++i)
	{
		pContext->VSSetConstantBuffers(i, 1, mBuffers[i].GetAddressOf());
	}

	for (int i = 0; i < mTextures.size(); ++i)
	{
		pContext->VSSetShaderResources(i, 1, mTextures[i].GetAddressOf());
	}

}

PixelShader::PixelShader() :
Shader(ShaderKind::Pixel),
mPixelShader(nullptr)
{

}

PixelShader::~PixelShader()
{
}

HRESULT PixelShader::CreateShader(void* pData, UINT size)
{
	return gD3D->GetDevice()->CreatePixelShader(pData, size, nullptr, mPixelShader.GetAddressOf());
}

void PixelShader::SetShader()
{
	ID3D11DeviceContext* pContext = gD3D->GetContext();
	pContext->PSSetShader(mPixelShader.Get(), nullptr, 0);

	for (int i = 0; i < mBuffers.size(); ++i)
		pContext->PSSetConstantBuffers(i, 1, mBuffers[i].GetAddressOf());
	for (int i = 0; i < mTextures.size(); ++i)
		pContext->PSSetShaderResources(i, 1, mTextures[i].GetAddressOf());
}

GeometryShader::GeometryShader():Shader(Shader::Geometry)
{
	mGS = nullptr;
}

GeometryShader::~GeometryShader()
{

}

void GeometryShader::SetShader()
{
	ID3D11DeviceContext* pContext = gD3D->GetContext();
	pContext->GSSetShader(mGS.Get(), nullptr, 0);
	for (int i = 0; i < mBuffers.size(); i++)
		pContext->GSSetConstantBuffers(i, 1, mBuffers[i].GetAddressOf());
	for (int i = 0; i < mBuffers.size(); i++)
		pContext->GSSetShaderResources(i, 1, mTextures[i].GetAddressOf());
}

void GeometryShader::UnsetShader()
{
	gD3D->GetContext()->GSSetShader(nullptr, nullptr, 0);
}

HRESULT GeometryShader::CreateShader(void* pData, UINT size)
{
	return gD3D->GetDevice()->CreateGeometryShader(pData, size, NULL, mGS.GetAddressOf());
}



