#include "UI_Primitive.h"

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
		Color(0.5f, 0.5f, 0.5f, 1.0f),		// ����
		Color(1.0f, 1.0f, 1.0f, 1.0f),		// �\�ʐF
		Color(1.0f, 0.5f, 0.5f, 0.2f),		// ���ʔ���: specular power 1
		Color(0.0f, 0.0f, 0.0f, 0.0f)		// �������Ȃ�};
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
	mFilePath = filePath;
}

//void UI_Primitive::CreateTexture(std::shared_ptr<Texture> texture)
//{
//	mMaterial.tex = texture;
//}

void UI_Primitive::CreateTexture(const std::shared_ptr<Texture>& texture)
{
	mMaterial.tex = texture;
}

void UI_Primitive::Update()
{
	UpdateScale();
	UpdatePos();
	if (!isUseUVAnimation) { return; }
	mUvAnimation->UpdateUV();

}

void UI_Primitive::SetPosition(const DirectX::XMFLOAT3& pos) noexcept
{
	mTransform.SetPosition(pos);
	mOriginPos = {pos.x,pos.y};
}

void UI_Primitive::SetScale(float width, float height)
{
	mTransform.SetScaleXY(width, height);
	mOriginScale = { width,height };
	
}

void UI_Primitive::SetScale(const DirectX::XMFLOAT2& size)
{
	mTransform.SetScaleXY(size);
	mOriginScale = size;
}

void UI_Primitive::SetScale(float scale)
{
	mTransform.SetScale(scale, scale, scale);
	mOriginScale = { scale,scale };
}

void UI_Primitive::SetPosZ(float z) noexcept
{
	mTransform.SetPositionZ(z);
}

void UI_Primitive::SetPosition(float x, float y) noexcept
{
	mTransform.SetPosition(x, y);
	mOriginPos = { x,y };
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

void UI_Primitive::LoadDefShader()
{
	mDefPS = std::make_shared<PixelShader>();
	mDefVS = std::make_shared<VertexShader>();

	HR(mDefPS->LoadShaderFile("Assets/Shader/PS_DefaultUI.cso"));
	HR(mDefVS->LoadShaderFile("Assets/Shader/VS_DefaultUI.cso"));

}

void UI_Primitive::SetVertices(const std::vector<Vertex::VtxPosNormalTex>& vertices) noexcept
{
	mVertices = vertices;
}

void UI_Primitive::SetDefShader()
{
	if (isDefShader)
	{
		mVS = mDefVS.get();
		mPS = mDefPS.get();
	}
}

void UI_Primitive::ResetVerticesData()
{
	isResetVertex = true;
}

void UI_Primitive::ClearResetVertices()
{
	isResetVertex = false;
}

void UI_Primitive::UpdateScale()
{
	if (!gD3D->GetResized()) { return; }

	float viewWidth = static_cast<float>(gD3D->GetWinWidth());
	float viewHeight = static_cast<float>(gD3D->GetWinHeight());
	Vector3 ratio = { viewWidth / WIN_WIDTH,viewHeight / WIN_HEIGHT,1.0f };
	Vector3 scale = { ratio.x * mOriginScale.x,ratio.y * mOriginScale.y,1.0f };
	mTransform.SetScale(scale);
}

void UI_Primitive::UpdatePos()
{
	if (!gD3D->GetResized()) { return; }
	float viewWidth = static_cast<float>(gD3D->GetWinWidth());
	float viewHeight = static_cast<float>(gD3D->GetWinHeight());
	Vector3 ratio = { viewWidth / WIN_WIDTH,viewHeight / WIN_HEIGHT,1.0f };
	Vector3 pos = { mOriginPos.x,mOriginPos.y, mTransform.GetPosition().z };
	pos *= ratio;
	mTransform.SetPosition(pos);
}

