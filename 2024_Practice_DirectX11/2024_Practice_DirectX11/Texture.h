#pragma once
#include "D3DApp.h"

/// @brief テクスチャ
class Texture
{
public:
	Texture() = default;
	virtual ~Texture() = default;
	HRESULT Create(const char* fileName);
	HRESULT Create(DXGI_FORMAT format, UINT width, UINT height, const void* pData = nullptr);

	UINT GetWidth() const { return mWidth; };
	UINT GetHeight() const { return mHeight; };
	ID3D11ShaderResourceView* GetResource() const { return mSRV.Get(); };

protected:
	D3D11_TEXTURE2D_DESC MakeTexDesc(DXGI_FORMAT format, UINT width, UINT height);
	virtual HRESULT CreateResource(D3D11_TEXTURE2D_DESC& desc, const void* pData);

protected:
	UINT mWidth = 0;	///< 横幅
	UINT mHeight = 0;	///< 縦幅
	ComPtr<ID3D11ShaderResourceView> mSRV;
	ComPtr<ID3D11Texture2D> mTex;
};

/// @brief レンダーターゲット
class RenderTarget : public Texture
{
public:
	RenderTarget() = default;
	~RenderTarget() = default;
	void Clear();
	void Clear(const float* color);
	HRESULT Create(DXGI_FORMAT format, UINT width, UINT height);
	HRESULT CreateFromScreen();
	ID3D11RenderTargetView* GetView() const { return mRTV.Get(); };

protected:
	virtual HRESULT CreateResource(D3D11_TEXTURE2D_DESC& desc, const void* pData = nullptr);

private:
	ComPtr<ID3D11RenderTargetView> mRTV = nullptr;
};

/// @brief 深度テクスチャ
class DepthStencil : public Texture
{
public:
	DepthStencil() = default;
	~DepthStencil() = default;
	void Clear();
	HRESULT Create(UINT width, UINT height, bool useStencil);
	ID3D11DepthStencilView* GetView() const { return mDSV.Get(); };

protected:
	virtual HRESULT CreateResource(D3D11_TEXTURE2D_DESC& desc, const void* pData = nullptr);

private:
	ComPtr<ID3D11DepthStencilView> mDSV = nullptr;
};
