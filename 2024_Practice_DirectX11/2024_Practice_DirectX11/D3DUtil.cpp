#include "d3dUtil.h"
using json = nlohmann::json;

HRESULT LoadTextureFromFile(ID3D11Device* pDevice, const char* pszFileName, ID3D11ShaderResourceView** ppTexture)
{
	HRESULT hr;
	DirectX::TexMetadata mdata;
	DirectX::ScratchImage image;
	wchar_t wPath[MAX_PATH];
	size_t wLen = 0;

	MultiByteToWideChar(0, 0, pszFileName, -1, wPath, MAX_PATH);
	if (strstr(pszFileName, ".tga"))
	{
		hr = DirectX::LoadFromTGAFile(wPath, &mdata, image);
	}
	else
	{
		hr = DirectX::LoadFromWICFile(wPath, DirectX::WIC_FLAGS::WIC_FLAGS_NONE, &mdata, image);
	}
	if (FAILED(hr)) {
		return hr;
	}

	hr = CreateShaderResourceView(
		pDevice,
		image.GetImages(), image.GetImageCount(),
		mdata, ppTexture);

	return hr;

}

