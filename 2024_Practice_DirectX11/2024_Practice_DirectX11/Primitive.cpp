﻿#include "Primitive.h"

#include "GameApp.h"
#include "KInput.h"
using namespace DirectX;
using namespace DirectX::SimpleMath;


enum DebugState
{
	STATE_NONE,
	STATE_CHECK,
	//todo:追加物体移动
};

Primitive::Primitive(PrimitiveConfig::PrimitiveKind kind) :mKind(kind)
{
}

Primitive::~Primitive()
{
}



void Primitive::CreateMaterial()
{
	mMaterial.material =
	{
		Color(0.5f, 0.5f, 0.5f, 1.0f),		// 環境光
		Color(1.0f, 1.0f, 1.0f, 1.0f),		// 表面色
		Color(1.0f, 0.5f, 0.5f, 0.2f),		// 鏡面反射: specular power 1
		Color(0.0f, 0.0f, 0.0f, 0.0f)		// 自発光なし};
	};

}

void Primitive::CreateTexture(const char* filePath)
{
	if (!filePath)
	{
		mMaterial.material.isTexEnable = false;
		mMaterial.tex = nullptr;
		return;
	}

	mMaterial.tex = std::make_shared<Texture>();
	HRESULT hr = mMaterial.tex->Create(filePath);
	if (FAILED(hr))
	{
		mMaterial.tex = nullptr;
		mMaterial.material.isTexEnable = false;
	}
}


void Primitive::SetScaleXZ(const DirectX::XMFLOAT2& scale) noexcept
{
	mTransform.SetScale(scale.x, 1.0f, scale.y);
}

void Primitive::SetScaleXY(const DirectX::XMFLOAT2& scale) noexcept
{
	mTransform.SetScale(scale.x, scale.y, 1.0f);
}

void Primitive::LoadTexture(const std::shared_ptr<Texture>& tex)
{
	if (!tex)
		mMaterial.material.isTexEnable = false;
	mMaterial.tex = tex;
}

void Primitive::SetPixelShader(PixelShader* ps) noexcept
{
	isDefShader = false;
	mPS = ps;
}

void Primitive::SetVertexShader(VertexShader* vs) noexcept
{
	isDefShader = false;
	mVS = vs;
}

void Primitive::LoadDefShader()
{
	mDefPS = std::make_shared<PixelShader>();
	mDefVS = std::make_shared<VertexShader>();

	mDefPS = SceneManager::Get()->GetObj<PixelShader>("PS_Primitives");
	mDefVS = SceneManager::Get()->GetObj<VertexShader>("VS_Primitives");
}

void Primitive::LoadDefShader(const char* vsPath, const char* psPath)
{
	mDefPS = std::make_shared<PixelShader>();
	mDefVS = std::make_shared<VertexShader>();
	mDefPS->LoadShaderFile(psPath);
	mDefVS->LoadShaderFile(vsPath);

}

void Primitive::LoadDefShader(const std::shared_ptr<VertexShader>& vsShader, const std::shared_ptr<PixelShader>& psShader)
{
	mDefPS = std::make_shared<PixelShader>();
	mDefVS = std::make_shared<VertexShader>();
	mDefPS = psShader;
	mDefVS = vsShader;
}


void Primitive::SwitchToDefShader()
{
	isDefShader = true;
}

void Primitive::SetDefShader()
{
	if (isDefShader)
	{
		WriteDefShader();
		mVS = mDefVS.get();
		mPS = mDefPS.get();
	}

#ifdef _DEBUG
	if (!mVS)
		DebugLog::LogError("VertexShader is null!");
	if (!mPS)
		DebugLog::LogError("PixelShader is null!");

#endif

}

void Primitive::SetVertices(std::vector<Vertex::VtxPosNormalTex> vertices) noexcept
{
	mVertices = vertices;
}

void Primitive::SetIndices(std::vector<DWORD> indices) noexcept
{
	mIndices = indices;
}




