#include "Texture.h"

HRESULT Texture::Create(const char* fileName)
{
	HRESULT hr = S_OK;
	
	// �����ϊ�
	wchar_t wPath[MAX_PATH];
	size_t wLen = 0;
	MultiByteToWideChar(0, 0, fileName, -1, wPath, MAX_PATH);

	// �t�@�C���ʓǂݍ���
	DirectX::TexMetadata mdata;
	DirectX::ScratchImage image;
	if (strstr(fileName, ".tga"))
		hr = DirectX::LoadFromTGAFile(wPath, &mdata, image);
	else
		hr = DirectX::LoadFromWICFile(wPath, DirectX::WIC_FLAGS::WIC_FLAGS_NONE, &mdata, image);
	if (FAILED(hr)) {
		return E_FAIL;
	}

	// �V�F�[�_���\�[�X����
	hr = CreateShaderResourceView(gD3D->GetDevice(), image.GetImages(), image.GetImageCount(), mdata, mSRV.GetAddressOf());
	if (SUCCEEDED(hr))
	{
		mWidth = (UINT)mdata.width;
		mHeight = (UINT)mdata.height;
	}
	return hr;
}
HRESULT Texture::Create(DXGI_FORMAT format, UINT width, UINT height, const void* pData)
{
	D3D11_TEXTURE2D_DESC desc = MakeTexDesc(format, width, height);
	return CreateResource(desc, pData);
}

D3D11_TEXTURE2D_DESC Texture::MakeTexDesc(DXGI_FORMAT format, UINT width, UINT height)
{
	D3D11_TEXTURE2D_DESC desc = {};
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.Format = format;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.Width = width;
	desc.Height = height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.SampleDesc.Count = 1;
	return desc;
}
HRESULT Texture::CreateResource(D3D11_TEXTURE2D_DESC& desc, const void* pData)
{
	HRESULT hr = E_FAIL;

	// �e�N�X�`���쐬
	D3D11_SUBRESOURCE_DATA data = {};
	data.pSysMem = pData;
	data.SysMemPitch = desc.Width * 4;
	hr = gD3D->GetDevice()->CreateTexture2D(&desc, pData ? &data : nullptr, &mTex);
	if (FAILED(hr)) { return hr; }

	// �ݒ�
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	switch (desc.Format)
	{
	default:						srvDesc.Format = desc.Format;			break;
	case DXGI_FORMAT_R32_TYPELESS: 	srvDesc.Format = DXGI_FORMAT_R32_FLOAT;	break;
	}
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	// ����
	hr = gD3D->GetDevice()->CreateShaderResourceView(mTex.Get(), &srvDesc, mSRV.GetAddressOf());
	if (SUCCEEDED(hr))
	{
		mWidth = desc.Width;
		mHeight = desc.Height;
	}
	return hr;
}

/// <summary>
/// �����_�[�^�[�Q�b�g
/// </summary>

void RenderTarget::Clear()
{
	static float color[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	Clear(color);
}
void RenderTarget::Clear(const float* color)
{
	gD3D->GetContext()->ClearRenderTargetView(mRTV.Get(), color);
}
HRESULT RenderTarget::Create(DXGI_FORMAT format, UINT width, UINT height)
{
	D3D11_TEXTURE2D_DESC desc = MakeTexDesc(format, width, height);
	desc.BindFlags |= D3D11_BIND_RENDER_TARGET;
	return CreateResource(desc);
}
HRESULT RenderTarget::CreateFromScreen()
{
	HRESULT hr;

	// �o�b�N�o�b�t�@�̃|�C���^���擾
	ID3D11Texture2D* pBackBuffer = NULL;
	hr = gD3D->GetSwapChain()->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&mTex);
	if (FAILED(hr)) { return hr; }

	// �o�b�N�o�b�t�@�ւ̃|�C���^���w�肵�ă����_�[�^�[�Q�b�g�r���[���쐬
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	rtvDesc.Texture2D.MipSlice = 0;
	hr = gD3D->GetDevice()->CreateRenderTargetView(mTex.Get(), &rtvDesc, mRTV.GetAddressOf());
	if (SUCCEEDED(hr))
	{
		D3D11_TEXTURE2D_DESC desc;
		mTex->GetDesc(&desc);
		mWidth = desc.Width;
		mHeight = desc.Height;
	}
	return hr;
}

HRESULT RenderTarget::CreateResource(D3D11_TEXTURE2D_DESC& desc, const void* pData)
{
	// �e�N�X�`�����\�[�X�쐬
	HRESULT hr = Texture::CreateResource(desc, nullptr);
	if (FAILED(hr)) { return hr; }

	// �ݒ�
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
	rtvDesc.Format = desc.Format;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

	// ����
	return 	gD3D->GetDevice()->CreateRenderTargetView(mTex.Get(), &rtvDesc, mRTV.GetAddressOf());
}

/// <summary>
/// �[�x�e�N�X�`��
/// </summary>
void DepthStencil::Clear()
{
	gD3D->GetContext()->ClearDepthStencilView(mDSV.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}
HRESULT DepthStencil::Create(UINT width, UINT height, bool useStencil)
{
	// https://docs.microsoft.com/ja-jp/windows/win32/direct3d11/d3d10-graphics-programming-guide-depth-stencil#compositing
	D3D11_TEXTURE2D_DESC desc = MakeTexDesc(useStencil ? DXGI_FORMAT_R24G8_TYPELESS : DXGI_FORMAT_R32_TYPELESS, width, height);
	desc.BindFlags |= D3D11_BIND_DEPTH_STENCIL;
	return CreateResource(desc);
}

HRESULT DepthStencil::CreateResource(D3D11_TEXTURE2D_DESC& desc, const void* pData)
{
	// �X�e���V���g�p����
	bool useStencil = (desc.Format == DXGI_FORMAT_R24G8_TYPELESS);

	// ���\�[�X����
	desc.BindFlags |= D3D11_BIND_DEPTH_STENCIL;
	HRESULT hr = Texture::CreateResource(desc, nullptr);
	if (FAILED(hr)) { return hr; }

	// �ݒ�
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = useStencil ? DXGI_FORMAT_D24_UNORM_S8_UINT : DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

	// ����
	return 	gD3D->GetDevice()->CreateDepthStencilView(mTex.Get(), &dsvDesc, mDSV.GetAddressOf());
}