#pragma once
#include "D3DApp.h"
#include "Shader.h"

/// @brief ��{�`��쐬
class Geometry
{
private:
	struct Vertex
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT4 color;
		DirectX::XMFLOAT3 normal;
	};

	struct Data
	{
		std::shared_ptr<VertexShader> objShader;
		std::shared_ptr<PixelShader> defPS;
	};
};

