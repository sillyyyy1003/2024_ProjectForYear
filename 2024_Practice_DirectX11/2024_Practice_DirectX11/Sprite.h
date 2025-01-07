#pragma once
#include <memory>
#include "Shader.h"
#include "Mesh.h"
#include "Texture.h"
#include "Transform.h"


class Sprite
{
private:

	struct Data
	{
		std::shared_ptr<Mesh> mesh;
		std::shared_ptr<Texture> tex;
		DirectX::XMFLOAT4X4 WVP[3];
		DirectX::XMFLOAT4 Param[3];
		VertexShader* VS;
		PixelShader* PS;
	};

	static Data mData;

	static std::shared_ptr<VertexShader> mDefVS;
	static std::shared_ptr<PixelShader> mDefPS;

	static std::shared_ptr<Texture> mWhiteTex;
	static bool isDefShader;



public:
	static void Init();
	static void UnInit();
	static void Draw();

	static void SetVertexShader(VertexShader* shader);
	static void SetPixelShader(PixelShader* shader);

	static void SetOffset(DirectX::XMFLOAT2 offset);
	static void SetSize(DirectX::XMFLOAT2 size);
	static void SetUVPos(DirectX::XMFLOAT2 pos);
	static void SetUVScale(DirectX::XMFLOAT2 scale);
	static void SetColor(DirectX::XMFLOAT4 color);
	static void SetTexture(const std::shared_ptr<Texture>& tex);
	

	static void SetWorld(DirectX::XMFLOAT4X4 world);
	static void SetView(DirectX::XMFLOAT4X4 view);
	static void SetProjection(DirectX::XMFLOAT4X4 proj);

};

