#pragma once
#include <wrl/client.h>
#include <d3d11_1.h>

using namespace Microsoft::WRL;

class RenderState
{
public:
    static ComPtr<ID3D11RasterizerState> RSWireframe;		            // wireFrame
    static ComPtr<ID3D11RasterizerState> RSNoCull;			            // 正反両面レンダリング
    static ComPtr<ID3D11RasterizerState> RSCullClockWise;	            // 正面だけ
    static ComPtr<ID3D11RasterizerState> RSShadow;						// 影用

    static ComPtr<ID3D11SamplerState> SSPointClamp;						// 采样器状态：点过滤与Clamp模式
    static ComPtr<ID3D11SamplerState> SSLinearWrap;			            // 采样器状态：线性过滤与Wrap模式
    static ComPtr<ID3D11SamplerState> SSLinearClamp;					// 采样器状态：线性过滤与Clamp模式
    /*
	static ComPtr<ID3D11SamplerState> SSAnisotropicWrap16x;		        // 采样器状态：16倍各向异性过滤与Wrap模式
    static ComPtr<ID3D11SamplerState> SSAnisotropicClamp2x;		        // 采样器状态：2倍各向异性过滤与Clamp模式
    static ComPtr<ID3D11SamplerState> SSAnisotropicClamp4x;		        // 采样器状态：4倍各向异性过滤与Clamp模式
    static ComPtr<ID3D11SamplerState> SSAnisotropicClamp8x;		        // 采样器状态：8倍各向异性过滤与Clamp模式
    static ComPtr<ID3D11SamplerState> SSAnisotropicClamp16x;		    // 采样器状态：16倍各向异性过滤与Clamp模式
    */
    static ComPtr<ID3D11SamplerState> SSShadowPCF;						// 采样器状态：深度比较与Border模式

    static ComPtr<ID3D11BlendState> BSTransparent;		                // Alpha blend
    static ComPtr<ID3D11BlendState> BSAlphaToCoverage;	                // Alpha-To-Coverage(MSAA)
    static ComPtr<ID3D11BlendState> BSAdditive;			                // Additive
    static ComPtr<ID3D11BlendState> BSAlphaWeightedAdditive;            // AlphaAddictive
    static ComPtr<ID3D11BlendState> BSColorDodge;                       // Color dodge
    static ComPtr<ID3D11BlendState> BSMulti;                            // 乗算

    //todo: MAKE THIS IN ENGLISH OR JAPANESE
    static ComPtr<ID3D11DepthStencilState> DSSEqual;					// 深度/模板状态：仅允许绘制深度值相等的像素
    static ComPtr<ID3D11DepthStencilState> DSSLessEqual;                // 深度/模板状态：用于传统方式天空盒绘制
    static ComPtr<ID3D11DepthStencilState> DSSGreaterEqual;             // 深度/模板状态：用于反向Z绘制
    static ComPtr<ID3D11DepthStencilState> DSSNoDepthWrite;             // 深度/模板状态：仅测试，但不写入深度值
    static ComPtr<ID3D11DepthStencilState> DSSNoDepthTest;              // 深度/模板状态：关闭深度测试
    static ComPtr<ID3D11DepthStencilState> DSSWriteStencil;		        // 深度/模板状态：无深度测试，写入模板值
    static ComPtr<ID3D11DepthStencilState> DSSEqualStencil;	            // 深度/模板状态：反向Z，检测模板值

public:
	static bool IsInit();
	static void InitAll(ID3D11Device* device);


};

