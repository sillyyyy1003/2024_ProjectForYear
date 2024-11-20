#pragma once
#include <d3d11.h>
#include <wrl/client.h>

using namespace Microsoft::WRL;
/// @brief テクスチャ
class Texture
{
public:
	Texture();
	virtual ~Texture();
	HRESULT Create(const char* fileName);
	HRESULT Create(DXGI_FORMAT format, UINT width, UINT height, const void* pData = nullptr);

	UINT GetWidth() const { return mWidth; };
	UINT GetHeight() const { return mHeight; };
	ID3D11ShaderResourceView* GetResource() const { return mSRV.Get(); };
	virtual HRESULT CreateResource(D3D11_TEXTURE2D_DESC& desc, const void* pData);
protected:
	D3D11_TEXTURE2D_DESC MakeTexDesc(DXGI_FORMAT format, UINT width, UINT height, UINT sampleCount = 1, UINT sampleQuality = 0);


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
	RenderTarget();
	~RenderTarget() override;
	void Clear();
	void Clear(const float* color);
	HRESULT Create(DXGI_FORMAT format, UINT width, UINT height, UINT sampleCount = 1, UINT sampleQuality = 0);
	HRESULT CreateFromScreen();
	ID3D11RenderTargetView* GetView() const { return mRTV.Get(); };

protected:
	virtual HRESULT CreateResource(D3D11_TEXTURE2D_DESC& desc, const void* pData = nullptr);

private:
	ComPtr<ID3D11RenderTargetView> mRTV;
};

/// @brief 深度テクスチャ
class DepthStencil : public Texture
{
public:
	DepthStencil();
	~DepthStencil() override;
	void Clear();
	HRESULT Create(UINT width, UINT height, bool useStencil, bool isEnable4xMsaa = true, UINT msaaQuality = 0);
	ID3D11DepthStencilView* GetView() const { return mDSV.Get(); };

protected:
	virtual HRESULT CreateResource(D3D11_TEXTURE2D_DESC& desc, const void* pData = nullptr);

private:
	ComPtr<ID3D11DepthStencilView> mDSV;
};
