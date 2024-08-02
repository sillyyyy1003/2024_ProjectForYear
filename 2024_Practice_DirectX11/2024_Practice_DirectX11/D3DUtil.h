//***************************************************************************************
//D3D Tool
//***************************************************************************************

#pragma once

#include <d3d11.h>            // include Windows.h
#include <DirectXCollision.h>   // include DirectXMath.h
#include <DirectXPackedVector.h>
#include <DirectXColors.h>
#include <d3dcompiler.h>
#include <vector>
#include <string>
#include <memory>
#include <SimpleMath.h>
#include "DXTrace.h"
#include "Mesh.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "imgui.lib")

//
// 宏相关
//


#ifndef GRAPHICS_DEBUGGER_OBJECT_NAME
#define GRAPHICS_DEBUGGER_OBJECT_NAME (0)
#endif

#define SAFE_RELEASE(p) { if ((p)) { (p)->Release(); (p) = nullptr; } }

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

namespace Vertex
{
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


	struct VtxPosNormalTexColor
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT4 color;
		DirectX::XMFLOAT2 tex;
	};

	struct VxPosNormalTangentColor
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT4 tangent;
		DirectX::XMFLOAT4 color;
	};

	struct Vtx
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT4 tangent;
		DirectX::XMFLOAT4 color;
		DirectX::XMFLOAT2 tex;	//Texture coordination;
	};

	enum PrintMode
	{
		Wireframe,//Modelのラインを表示
		Solid,//Modelの面を表示
	};

	//描画方法のリスト
	static std::unordered_map<PrintMode, D3D11_PRIMITIVE_TOPOLOGY> TopologyList = {
		{Wireframe, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST},
		{Solid,D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST},
	};

}


//todo
struct Material
{
	DirectX::XMFLOAT4 ambient;	// 環境光 ka
	DirectX::XMFLOAT4 diffuse;	// 拡散反射 kd
	DirectX::XMFLOAT4 specular; // 鏡面反射 ks 
	//specular={ specPower スペキュラの絞り,metallic メタリック,smooth スムース,blank}
	DirectX::XMFLOAT4 emission; // 反射 ke
	
};

struct DirectionLight
{
	DirectX::XMFLOAT4 ambient;	// 環境光 
	DirectX::XMFLOAT4 diffuse;	// 拡散反射
	DirectX::XMFLOAT4 specular; // 鏡面反射 
	DirectX::XMFLOAT3 direction;
	float isEnable;//起動するかどうか


	
	
};

struct PointLight
{
	DirectX::XMFLOAT4 ambient;	// 環境光 
	DirectX::XMFLOAT4 diffuse;	// 拡散反射
	DirectX::XMFLOAT4 specular; // 鏡面反射 
	DirectX::XMFLOAT3 position;
	float range;

	DirectX::XMFLOAT3  attenuation;//減衰
	float isEnable;//起動するかどうか

	
};

struct Spotlight
{
	
	DirectX::XMFLOAT4 ambient;	// 環境光 
	DirectX::XMFLOAT4 diffuse;	// 拡散反射 
	DirectX::XMFLOAT4 specular; // 鏡面反射

	DirectX::XMFLOAT3 position;
	float range;

	DirectX::XMFLOAT3 direction; //向かうどころ
	float Spot;	//焦点值，较高的值表示更聚焦的光束，较低的值表示更扩散的光束


	DirectX::XMFLOAT3  attenuation;//減衰
	float isEnable;//起動するかどうか
};