//***************************************************************************************
//D3D Tool
//***************************************************************************************

#pragma once

#include <d3d11.h>				// include Windows.h
#include <DirectXCollision.h>   // include DirectXMath.h
#include <DirectXPackedVector.h>
#include <DirectXColors.h>
#include <d3dcompiler.h>
#include <vector>
#include <string>
#include <memory>
#include <SimpleMath.h>
#include <imgui/Include/imgui.h>
#include <imgui/Include/imgui_impl_dx11.h>
#include <imgui/Include/imgui_impl_win32.h>
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

/// @brief 逆行列を求める
///	@param ansmtx 求める逆行列
///	@param mtx 元の行列
inline void MtxInverse(DirectX::XMFLOAT4X4& ansmtx, const DirectX::XMFLOAT4X4& mtx)
{
	ALIGN16 DirectX::XMMATRIX mat, matans;
	mat = XMLoadFloat4x4(&mtx);
	matans = DirectX::XMMatrixInverse(nullptr, mat);
	XMStoreFloat4x4(&ansmtx, matans);
}

/// @brief 単位行列を求める
/// @param mat 変換したい行列
inline void MtxIdentity(DirectX::XMFLOAT4X4& mat)
{
	ALIGN16 DirectX::XMMATRIX mtx;
	mtx = DirectX::XMMatrixIdentity();
	XMStoreFloat4x4(&mat, mtx);
}

/// @brief 行列の掛け算をする
/// @param ansmtx 結果
/// @param p1mtx 掛け算用行列1
/// @param p2mtx 掛け算用行列2
inline void MtxMultiply(DirectX::XMFLOAT4X4& ansmtx, const DirectX::XMFLOAT4X4& p1mtx, const DirectX::XMFLOAT4X4& p2mtx)
{
	ALIGN16 DirectX::XMMATRIX mat1, mat2, matans;

	mat1 = XMLoadFloat4x4(&p1mtx);
	mat2 = XMLoadFloat4x4(&p2mtx);

	matans = XMMatrixMultiply(mat1, mat2);

	XMStoreFloat4x4(&ansmtx, matans);
}


/// @brief テクスチャを読み込み
/// @param pDevice d3d->device
/// @param pszFileName texture filename
/// @param ppTexture texture pointer
/// @return 
HRESULT LoadTextureFromFile(ID3D11Device* pDevice,const char* pszFileName, ID3D11ShaderResourceView** ppTexture);


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
		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT4 color;

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
		DirectX::XMFLOAT4 color;
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


//todo
struct Material
{
	DirectX::XMFLOAT4 ambient = { 0.5f, 0.5f, 0.5f, 0.5f };	// 環境光 ka
	DirectX::XMFLOAT4 diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };	// 拡散反射 kd
	DirectX::XMFLOAT4 specular = { 1.0f, 0.5f, 0.5f, 0.2f }; // 鏡面反射 ks 
	DirectX::XMFLOAT4 emission = { 0.0f, 0.0f, 0.0f, 0.0f }; // 反射 ke 自発光なし
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