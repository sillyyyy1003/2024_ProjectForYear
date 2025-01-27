//***************************************************************************************
//D3D Tool
//***************************************************************************************

#pragma once

#include <d3d11.h>				// include Windows.h
#include <DirectXCollision.h>   // include DirectXMath.h
#include <DirectXPackedVector.h>
#include <DirectXColors.h>
#include <d3dcompiler.h>
#include <filesystem>
#include <vector>
#include <string>
#include <memory>
#include <SimpleMath.h>
#include <imgui/Include/imgui.h>
#include <imgui/Include/imgui_impl_dx11.h>
#include <imgui/Include/imgui_impl_win32.h>
#include <nlohmann/json_fwd.hpp>

#include "DebugLog.h"
#include "DXTrace.h"
#include "Mesh.h"
#include "DirectXTex.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "imgui.lib")
#pragma comment(lib, "DirectXTex.lib")
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "DirectXTK.lib")


// 16バイトにアラインメントする。
#define ALIGN16 _declspec(align(16))

//
// 宏相关
//

#ifndef GRAPHICS_DEBUGGER_OBJECT_NAME
#define GRAPHICS_DEBUGGER_OBJECT_NAME (0)
#endif

#define SAFE_RELEASE(p) { if ((p)) { (p)->Release(); (p) = nullptr; } }
#define WIN_WIDTH	(1920.0f)
#define WIN_HEIGHT	(1080.0f)
#define UI_NEARZ	(0.0f)
#define UI_FARZ		(3.0f)
//
// 辅助调试相关函数
//

// ------------------------------
// D3D11SetDebugObjectName函数
// ------------------------------
// 为D3D设备创建出来的对象在图形调试器中设置对象名
// [In]resource				D3D11设备创建出的对象
// [In]name					对象名
template<UINT TNameLength>
inline void D3D11SetDebugObjectName(_In_ ID3D11DeviceChild* resource, _In_ const char(&name)[TNameLength])
{
#if (defined(DEBUG) || defined(_DEBUG)) && (GRAPHICS_DEBUGGER_OBJECT_NAME)
    resource->SetPrivateData(WKPDID_D3DDebugObjectName, TNameLength - 1, name);
#else
    UNREFERENCED_PARAMETER(resource);
    UNREFERENCED_PARAMETER(name);
#endif
}

// ------------------------------
// D3D11SetDebugObjectName函数
// ------------------------------
// 为D3D设备创建出来的对象在图形调试器中设置对象名
// [In]resource				D3D11设备创建出的对象
// [In]name					对象名
// [In]length				字符串长度
inline void D3D11SetDebugObjectName(_In_ ID3D11DeviceChild* resource, _In_ LPCSTR name, _In_ UINT length)
{
#if (defined(DEBUG) || defined(_DEBUG)) && (GRAPHICS_DEBUGGER_OBJECT_NAME)
    resource->SetPrivateData(WKPDID_D3DDebugObjectName, length, name);
#else
    UNREFERENCED_PARAMETER(resource);
    UNREFERENCED_PARAMETER(name);
    UNREFERENCED_PARAMETER(length);
#endif
}

// ------------------------------
// D3D11SetDebugObjectName函数
// ------------------------------
// 为D3D设备创建出来的对象在图形调试器中设置对象名
// [In]resource				D3D11设备创建出的对象
// [In]name					对象名
inline void D3D11SetDebugObjectName(_In_ ID3D11DeviceChild* resource, _In_ const std::string& name)
{
#if (defined(DEBUG) || defined(_DEBUG)) && (GRAPHICS_DEBUGGER_OBJECT_NAME)
    resource->SetPrivateData(WKPDID_D3DDebugObjectName, (UINT)name.length(), name.c_str());
#else
    UNREFERENCED_PARAMETER(resource);
    UNREFERENCED_PARAMETER(name);
#endif
}

// ------------------------------
// D3D11SetDebugObjectName函数
// ------------------------------
// 为D3D设备创建出来的对象在图形调试器中清空对象名
// [In]resource				D3D11设备创建出的对象
inline void D3D11SetDebugObjectName(_In_ ID3D11DeviceChild* resource, _In_ std::nullptr_t)
{
#if (defined(DEBUG) || defined(_DEBUG)) && (GRAPHICS_DEBUGGER_OBJECT_NAME)
    resource->SetPrivateData(WKPDID_D3DDebugObjectName, 0, nullptr);
#else
    UNREFERENCED_PARAMETER(resource);
#endif
}

// ------------------------------
// DXGISetDebugObjectName函数
// ------------------------------
// 为DXGI对象在图形调试器中设置对象名
// [In]object				DXGI对象
// [In]name					对象名
template<UINT TNameLength>
inline void DXGISetDebugObjectName(_In_ IDXGIObject* object, _In_ const char(&name)[TNameLength])
{
#if (defined(DEBUG) || defined(_DEBUG)) && (GRAPHICS_DEBUGGER_OBJECT_NAME)
    object->SetPrivateData(WKPDID_D3DDebugObjectName, TNameLength - 1, name);
#else
    UNREFERENCED_PARAMETER(object);
    UNREFERENCED_PARAMETER(name);
#endif
}

// ------------------------------
// DXGISetDebugObjectName函数
// ------------------------------
// 为DXGI对象在图形调试器中设置对象名
// [In]object				DXGI对象
// [In]name					对象名
// [In]length				字符串长度
inline void DXGISetDebugObjectName(_In_ IDXGIObject* object, _In_ LPCSTR name, _In_ UINT length)
{
#if (defined(DEBUG) || defined(_DEBUG)) && (GRAPHICS_DEBUGGER_OBJECT_NAME)
    object->SetPrivateData(WKPDID_D3DDebugObjectName, length, name);
#else
    UNREFERENCED_PARAMETER(object);
    UNREFERENCED_PARAMETER(name);
    UNREFERENCED_PARAMETER(length);
#endif
}

// ------------------------------
// DXGISetDebugObjectName函数
// ------------------------------
// 为DXGI对象在图形调试器中设置对象名
// [In]object				DXGI对象
// [In]name					对象名
inline void DXGISetDebugObjectName(_In_ IDXGIObject* object, _In_ const std::string& name)
{
#if (defined(DEBUG) || defined(_DEBUG)) && (GRAPHICS_DEBUGGER_OBJECT_NAME)
    object->SetPrivateData(WKPDID_D3DDebugObjectName, (UINT)name.length(), name.c_str());
#else
    UNREFERENCED_PARAMETER(object);
    UNREFERENCED_PARAMETER(name);
#endif
}

// ------------------------------
// DXGISetDebugObjectName函数
// ------------------------------
// 为DXGI对象在图形调试器中清空对象名
// [In]object				DXGI对象
inline void DXGISetDebugObjectName(_In_ IDXGIObject* object, _In_ std::nullptr_t)
{
#if (defined(DEBUG) || defined(_DEBUG)) && (GRAPHICS_DEBUGGER_OBJECT_NAME)
    object->SetPrivateData(WKPDID_D3DDebugObjectName, 0, nullptr);
#else
    UNREFERENCED_PARAMETER(object);
#endif
}

inline void Error(const char* format, ...)
{
	va_list arg;
	va_start(arg, format);
	static char buf[1024];
	vsprintf_s(buf, sizeof(buf), format, arg);
	va_end(arg);
	MessageBoxA(NULL, buf, "Error", MB_OK);
}


/// @brief テクスチャを読み込み
/// @param pDevice d3d->device
/// @param pszFileName texture filename
/// @param ppTexture texture pointer
/// @return 
HRESULT LoadTextureFromFile(ID3D11Device* pDevice,const char* pszFileName, ID3D11ShaderResourceView** ppTexture);


inline DirectX::XMFLOAT2 LocalClientToWorld(const POINT& cursorPos, DirectX::XMFLOAT2 winSize)
{
	DirectX::XMFLOAT2 result;
	float winWidth = winSize.x;
	float winHeight = winSize.y;

	result.x = cursorPos.x - winWidth / 2.f;
	result.y = winHeight / 2.f - cursorPos.y;
	return result;
}


namespace Vertex
{
	struct VtxPosTex
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT2 tex;
	};

	struct VtxPosColorNormal
	{
		DirectX::XMFLOAT3 pos;		
		DirectX::XMFLOAT4 color;
		DirectX::XMFLOAT3 normal;

	};

	struct VtxPosColor
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT4 color;
	};

	struct VtxPosNormalTex
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT2 tex;
	};


	struct VtxPosNormalTexColor
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT2 tex;
		DirectX::XMFLOAT4 color = { 1.f,1.f,1.f,1.f };
	};

	struct VtxPosNormalTangentTex
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT2 tex;
		DirectX::XMFLOAT3 tangent;
	};

	struct Vtx
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT2 tex;	//Texture coordination;
		DirectX::XMFLOAT4 tangent;
		DirectX::XMFLOAT4 color;
		
	};

	enum PrintMode
	{
		Wireframe,//Modelのラインを表示
		Solid,//Modelの面を表示
	};

}


struct HSV
{
	float hue = 0.f;	//色相
	float saturation = 1.f;	//彩度
	float value = 1.f;	//明度
	float alpha = 1.f;	//透明度
};

inline DirectX::XMFLOAT4 HSVtoRGB(const HSV& color)
{
	float c = color.value * color.saturation;
	float x = c * (1.f - fabs(fmod(color.hue / 60.f, 2.f) - 1.f));
	float m = color.value - c;

	float r, g, b = 0.0f;
	DirectX::SimpleMath::Color outputColor;
	if (color.hue >= 0 && color.hue < 60.f)
	{
		r = c;
		g = x;
		b = 0;
	}
	else if (color.hue >= 60.f && color.hue < 120.f)
	{
		r = x;
		g = c;
		b = 0;
	}
	else if (color.hue >= 120.f && color.hue < 180.f)
	{
		r = 0;
		g = c;
		b = x;
	}
	else if (color.hue >= 180.f && color.hue < 240.f)
	{
		r = 0;
		g = x;
		b = c;
	}
	else if (color.hue >= 240.f && color.hue < 300.f)
	{
		r = x;
		g = 0;
		b = c;

	}
	else
	{
		r = c;
		g = 0;
		b = x;
	}

	outputColor.R(r + m);
	outputColor.G(g + m);
	outputColor.B(b + m);
	outputColor.A(color.alpha);
	return outputColor;
}

inline HSV RGBtoHSV(const DirectX::XMFLOAT4& color)
{
	float maxVal = std::max({ color.x,color.y,color.z });
	float minVal = std::min({ color.x,color.y,color.z });

	float delta = maxVal - minVal;
	float h = 0.f, s = 0.f, v = maxVal;

	if (delta > 0)
	{
		if (maxVal == color.x)
		{
			h = 60.f * (fmod(((color.y - color.z) / delta), 6.0f));
		}
		else if (maxVal == color.y)
		{
			h = 60.f * (((color.z - color.x) / delta) + 2.0f);
		}
		else
		{
			h = 60.f * (((color.x - color.y) / delta) + 4.f);
		}

		if (maxVal > 0)
		{
			s = delta / maxVal;
		}
	}
	if (h < 0.f)h += 360.f;

	return { h,s,v,color.w };
}

namespace ColorConfig
{
	constexpr DirectX::XMFLOAT4 DEFAULT_AMBIENT = { 0.2f, 0.2f, 0.2f, 1.0f };
	constexpr DirectX::XMFLOAT4 DEFAULT_DIFFUSE = { 1.0f, 1.0f, 1.0f, 1.0f };

	constexpr DirectX::XMFLOAT4 DEFAULT_SHADOW_COLOR = { 0.2f,0.2f,0.2f,0.6f };
	constexpr DirectX::XMFLOAT4 WHITE = { 1.0f, 1.0f, 1.0f, 1.0f };
	constexpr DirectX::XMFLOAT4 BLACK = { 0.0f, 0.0f, 0.0f, 1.0f };

}
//todo
struct Material
{
	DirectX::XMFLOAT4 ambient = ColorConfig::DEFAULT_AMBIENT;	// 環境光 ka
	DirectX::XMFLOAT4 diffuse = ColorConfig::DEFAULT_DIFFUSE;	// 拡散反射 kd
	DirectX::XMFLOAT4 specular = { 0.0f, 0.0f, 0.0f, 1.0f }; // 鏡面反射 ks 
	DirectX::XMFLOAT4 emission = {}; // 反射 ke 自発光なし
	float isTexEnable = true;
	float pad1, pad2, pad3 = 0.f;
};

namespace Light
{
	/// @brief  方向光
	struct DirectionLight
	{
		DirectX::XMFLOAT4 ambient;	// 環境光 
		DirectX::XMFLOAT4 diffuse;	// 拡散反射
		DirectX::XMFLOAT3 direction;
		float isEnable;//起動するかどうか

	};

	/// @brief 点光源
	struct PointLight
	{
		DirectX::XMFLOAT4 ambient = { 0.5f,0.5f,0.5f,1.0f };	// 環境光 
		DirectX::XMFLOAT4 diffuse = { 1.f,1.f,1.f,1.f };	// 拡散反射
		DirectX::XMFLOAT3 position = {};
		float range = 1;

		DirectX::XMFLOAT3  attenuation = { 1,0,0 };//減衰
		float isEnable = false;//起動するかどうか
	};

	/// @brief スポットライト
	struct Spotlight
	{

		DirectX::XMFLOAT4 ambient;	// 環境光 
		DirectX::XMFLOAT4 diffuse;	// 拡散反射 

		DirectX::XMFLOAT3 position;
		float range;

		DirectX::XMFLOAT3 direction; //向かうどころ
		float Spot;	//焦点值，较高的值表示更聚焦的光束，较低的值表示更扩散的光束

		DirectX::XMFLOAT3  attenuation;//減衰
		float isEnable;//起動するかどうか
	};

}

struct NormalConstantBuffer
{

	//CameraPos
	DirectX::XMFLOAT4 cameraPos={};

	//Environment Light
	DirectX::XMFLOAT4 lightAmbient={};
	DirectX::XMFLOAT4 lightDiffuse={};
	DirectX::XMFLOAT4 lightDir={};

	//Material
	Material mat = {};
};

struct UVConstantBuffer
{
	DirectX::XMMATRIX uv;
	int useUV;
	int dum1, dum2, dum3 = 0;
};

struct WVPConstantBuffer
{
	DirectX::XMFLOAT4X4 world;
	DirectX::XMFLOAT4X4 view;
	DirectX::XMFLOAT4X4 proj;
};