#include "UI_Primitive.h"

#include "GameApp.h"
#include "SceneManager.h"

UI_Primitive::UI_Primitive(UIPrimitiveConfig::UI_PrimitiveKind kind)
{
	mKind = kind;
}

UI_Primitive::~UI_Primitive()
{
}

void UI_Primitive::CreateMaterial()
{
	mMaterial.material =
	{
		Color(1.0f, 1.0f, 1.0f, 1.0f),		// ä¬ã´åı
		Color(1.0f, 1.0f, 1.0f, 1.0f),		// ï\ñ êF
		Color(1.0f, 0.5f, 0.5f, 0.2f),		// ãæñ îΩéÀ: specular power 1
		Color(0.0f, 0.0f, 0.0f, 0.0f)		// é©î≠åıÇ»Çµ};
	};
}

void UI_Primitive::CreateTexture(const char* filePath)
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


void UI_Primitive::CreateTexture(const std::shared_ptr<Texture>& texture)
{
	if (!texture)mMaterial.material.isTexEnable = false;
	mMaterial.tex = texture;
}

void UI_Primitive::CreateTexture(const std::shared_ptr<Texture>* pTex)
{
	if (*pTex == nullptr)mMaterial.material.isTexEnable = false;
	mMaterial.tex = *pTex;
}

void UI_Primitive::Update()
{
	if (!isUseUVAnimation) { return; }
	mUvAnimation->UpdateUV();
}

void UI_Primitive::SetPosition(const DirectX::XMFLOAT3& pos) noexcept
{
	mTransform.SetPosition(pos);
}

void UI_Primitive::SetScale(float width, float height)
{
	mTransform.SetScaleXY(width, height);

}

void UI_Primitive::SetScale(const DirectX::XMFLOAT2& size)
{
	mTransform.SetScaleXY(size);
}

void UI_Primitive::SetScale(float scale)
{
	mTransform.SetScale(scale, scale, scale);
}

void UI_Primitive::SetPosZ(float z) noexcept
{
	mTransform.SetPositionZ(z);
}

void UI_Primitive::SetPosition(float x, float y) noexcept
{
	mTransform.SetPosition(x, y);
}

void UI_Primitive::SetPosition(const DirectX::XMFLOAT2& pos) noexcept
{
	mTransform.SetPosition(pos.x, pos.y);
}

DirectX::XMFLOAT2 UI_Primitive::GetUIScale()
{
	return Vector2(mTransform.GetScale().x, mTransform.GetScale().y);
}

DirectX::XMFLOAT2 UI_Primitive::GetUIPosition()
{
	return Vector2{ mTransform.GetPosition().x,mTransform.GetPosition().y };
}

const DirectX::XMFLOAT4& UI_Primitive::GetDiffuseColor()
{
	return mMaterial.material.diffuse;
}

const Material& UI_Primitive::GetMaterial()
{
	return mMaterial.material;
}

void UI_Primitive::SetDiffuseColor(const DirectX::XMFLOAT4& color)
{
	mMaterial.material.diffuse = color;
}

void UI_Primitive::SetDiffuseColor(const float* color)
{
	mMaterial.material.diffuse = Color(color[0], color[1], color[2], color[3]);

}

void UI_Primitive::SetAmbientColor(const DirectX::XMFLOAT4& color)
{
	mMaterial.material.ambient = color;
}

void UI_Primitive::SetTexture(const std::shared_ptr<Texture>& texture)
{
	mMaterial.tex.reset();
	mMaterial.tex = texture;
}

void UI_Primitive::SetTransparency(float _transparency)
{
	mMaterial.material.diffuse.w = _transparency;
}

void UI_Primitive::LoadPSShader(const char* psShader)
{
	mDefPS.reset();
	mDefPS = std::make_shared<PixelShader>();
	HR(mDefPS->LoadShaderFile(psShader));

}

void UI_Primitive::LoadVSShader(const char* vsShader)
{
	mDefVS.reset();
	mDefVS = std::make_shared<VertexShader>();
	HR(mDefVS->LoadShaderFile(vsShader));
}

void UI_Primitive::LoadDefShader(const char* vsShader, const char* psShader)
{
	LoadVSShader(vsShader);
	LoadPSShader(psShader);
}

void UI_Primitive::LoadDefShader(const std::shared_ptr<VertexShader>& vs, const std::shared_ptr<PixelShader>& ps)
{
	mDefVS.reset();
	mDefVS = std::make_shared<VertexShader>();
	mDefVS = vs;

	mDefPS.reset();
	mDefPS = std::make_shared<PixelShader>();
	mDefPS = ps;
}

void UI_Primitive::LoadDefShader()
{
	mDefPS = std::make_shared<PixelShader>();
	mDefVS = std::make_shared<VertexShader>();

	mDefPS = SceneManager::Get()->GetObj<PixelShader>("PS_DefaultUI");
	mDefVS = SceneManager::Get()->GetObj<VertexShader>("VS_DefaultUI");

}

void UI_Primitive::SetVertices(const std::vector<Vertex::VtxPosNormalTex>& vertices) noexcept
{
	mVertices = vertices;
}

void UI_Primitive::SetDefShader()
{
	if (isDefShader)
	{
		WriteDefShader();
		mVS = mDefVS.get();
		mPS = mDefPS.get();
	}
}

