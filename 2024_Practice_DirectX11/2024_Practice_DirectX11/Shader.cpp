#include "Shader.h"
#include <cstdio>
#include "D3DUtil.h"
#include "DebugLog.h"
#include "GampApp.h"

#pragma comment(lib, "d3dcompiler.lib")

Shader::Shader(ShaderEnum::ShaderKind _shader) :mShader(_shader)
{
}


HRESULT Shader::CompileShader(const char* pCode)
{
	static const char* pTargetList[] =
	{
		"vs_5_0",
		"ps_5_0",
		//"gs_5_0",
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
	return hr;
}

void Shader::WriteShader(UINT slot, void* pData)
{
	//大きさを超えないよう
	if (slot < mBuffers.size())
	{
		if (mBuffers[slot] == nullptr)
		{
			DebugLog::LogError("nullptr");
		}
		gD3D->GetContext()->UpdateSubresource(mBuffers[slot].Get(), 0, nullptr, pData, 0, 0);
	}
}

/*
void Shader::SetTexture(UINT slot, Texture* _texture)
{
	if (slot >= mTextures.size()) { return; }
	ID3D11ShaderResourceView* pTex = _texture ? _texture->GetSRV() : nullptr;
	mTextures[slot] = pTex;
	switch (mShader)
	{
	case ShaderEnum::ShaderKind::Vertex:
		D3D::Get()->GetContext()->VSSetShaderResources(slot, 1, &pTex);
		break;
	case ShaderEnum::ShaderKind::Pixel:
		D3D::Get()->GetContext()->PSSetShaderResources(slot, 1, &pTex);
		break;
		//case Hull:		GetContext()->HSSetShaderResources(slot, 1, &pTex); break;
		//case Domain:	GetContext()->DSSetShaderResources(slot, 1, &pTex); break;
	}
}
*/

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
	mBuffers.resize(shaderDesc.ConstantBuffers, nullptr);
	
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

VertexShader::VertexShader(ShaderEnum::ShaderKind _shader) :
	Shader(ShaderEnum::ShaderKind::Vertex),
	mVertexShader(nullptr),
	mInputLayout(nullptr)
{


}

HRESULT VertexShader::CreateShader(void* pData, UINT size)
{
	HRESULT hr;
	ID3D11Device* pDevice = gD3D->GetDevice();
	//ID3D11VertexShader* vs;

	// シェーダー作成
	hr = pDevice->CreateVertexShader(pData, size, nullptr, mVertexShader.GetAddressOf());
	if (FAILED(hr)) { return hr; }

	
	/*
	シェーダ作成時にシェーダリフレクションを通してインプットレイアウトを取得
	セマンティクスの配置などから識別子を作成
	識別子が登録済→再利用、なければ新規作成
	https://blog.techlab-xe.net/dxc-shader-reflection/
	*/

	ID3D11ShaderReflection* pReflection = {};
	D3D11_SHADER_DESC shaderDesc;
	std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutDesc;

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

	for (UINT i = 0; i < shaderDesc.InputParameters; ++i)
	{
		D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
		pReflection->GetInputParameterDesc(i, &paramDesc);

		D3D11_INPUT_ELEMENT_DESC elementDesc;
		elementDesc.SemanticName = paramDesc.SemanticName;
		elementDesc.SemanticIndex = paramDesc.SemanticIndex;
		elementDesc.InputSlot = 0;
		elementDesc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		elementDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		elementDesc.InstanceDataStepRate = 0;

		/*switch(paramDesc.ComponentType)
		{
		case D3D_REGISTER_COMPONENT_UINT32:
			elementDesc.Format = DXGI_FORMAT_R32_UINT;
			break;
		case D3D_REGISTER_COMPONENT_SINT32:
			elementDesc.Format = DXGI_FORMAT_R32_SINT;
			break;
		case D3D_REGISTER_COMPONENT_FLOAT32:
			elementDesc.Format = DXGI_FORMAT_R32_FLOAT;
			break;
		}*/

		/*
		// Set Format
		if (paramDesc.Mask == 1)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
		}
		else if (paramDesc.Mask <= 3)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
		}
		else if (paramDesc.Mask <= 7)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
		}
		else if (paramDesc.Mask <= 15)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		}
		*/
		int formatIndex = 0;
		switch(paramDesc.ComponentType)
		{
		case D3D_REGISTER_COMPONENT_UINT32:formatIndex = 0; break;
		case D3D_REGISTER_COMPONENT_SINT32:formatIndex = 1; break;
		case D3D_REGISTER_COMPONENT_FLOAT32:formatIndex = 2; break;
		}

		// Determine the mask index
		int maskIndex = 0;
		if (paramDesc.Mask == 1) {
			maskIndex = 0;
		}
		else if (paramDesc.Mask <= 3) {
			maskIndex = 1;
		}
		else if (paramDesc.Mask <= 7) {
			maskIndex = 2;
		}
		else if (paramDesc.Mask <= 15) {
			maskIndex = 3;
		}

		// Set the format using the calculated indices
		elementDesc.Format = formats[formatIndex][maskIndex];
		inputLayoutDesc.push_back(elementDesc);
	}

	hr = pDevice->CreateInputLayout(&inputLayoutDesc[0], (UINT)inputLayoutDesc.size(), pData, size, mInputLayout.GetAddressOf());

	
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

PixelShader::PixelShader(ShaderEnum::ShaderKind _shader) :Shader(ShaderEnum::ShaderKind::Pixel),
mPixelShader(nullptr)
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


