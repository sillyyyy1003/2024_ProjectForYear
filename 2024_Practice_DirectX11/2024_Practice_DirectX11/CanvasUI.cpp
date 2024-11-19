#include "CanvasUI.h"
#include "FirstPersonCamera.h"
#include "GampApp.h"
#include "Shader.h"
#include "Texture.h"

using namespace DirectX::SimpleMath;
using namespace DirectX;

CanvasUI::~CanvasUI()
{
}

void CanvasUI::Init(const char* _fileName, DirectX::XMINT2 split)
{
	//UV Animation を使うか
	if (split.x == 1 && split.y == 1)
	{
		isUseUVAnimation = false;
	}else
	{
		isUseUVAnimation = true;
	}
	//UV Animationの初期化
	mUvAnimation = std::make_unique<UVAnimation>();
	mUvAnimation->Init(split);
	CreateMeshBuffer();
	CreateMaterial(_fileName);
	LoadShader();
}

void CanvasUI::Init(MaterialData& _materialData, DirectX::XMINT2 _split)
{
	//UV Animation を使うか
	if (_split.x == 1 && _split.y == 1)
	{
		isUseUVAnimation = false;
	}
	else
	{
		isUseUVAnimation = true;
	}
	//UV Animationの初期化
	mUvAnimation = std::make_unique<UVAnimation>();
	mUvAnimation->Init(_split);

	CreateMeshBuffer();
	CreateMaterial(_materialData);
	LoadShader();

}

void CanvasUI::Init(std::shared_ptr<Texture> tex, DirectX::XMINT2 _split)
{
	//UV Animation を使うか
	if (_split.x == 1 && _split.y == 1)
	{
		isUseUVAnimation = false;
	}
	else
	{
		isUseUVAnimation = true;
	}
	//UV Animationの初期化
	mUvAnimation = std::make_unique<UVAnimation>();
	mUvAnimation->Init(_split);


	CreateMeshBuffer();
	CreateMaterial(tex);
	LoadShader();


}


void CanvasUI::SetPosition(DirectX::XMFLOAT3 pos)
{
	mTransform.SetPosition(pos);
	mOriginPos = { pos.x,pos.y };
}



void CanvasUI::LoadSaveData(json data, const char* objName)
{
	//Scale
	Vector2 size = Vector2(data[objName]["Scale"][0], data[objName]["Scale"][1]);
	SetCanvasSize(size.x, size.y);

	//Pos
	Vector3 pos = Vector3(data[objName]["Position"][0], data[objName]["Position"][1], data[objName]["Position"][2]);
	SetPosition(pos);

	Vector3 rotation = Vector3(data[objName]["Rotation"][0], data[objName]["Rotation"][1], data[objName]["Rotation"][2]);
	mTransform.SetRotationInDegree(rotation);

	Init(data[objName]["Filepath"].get<std::string>().c_str());

	mObjectName = objName;

}

void CanvasUI::Update(float dt)
{
	UpdateScale();
	UpdatePos();
	if (!isUseUVAnimation) { return; }
	mUvAnimation->UpdateUV();
}

void CanvasUI::Update()
{
	UpdateScale();
	UpdatePos();
	if (!isUseUVAnimation) { return; }
	mUvAnimation->UpdateUV();
}

void CanvasUI::Draw()
{
	//============================================
	//Generate Matrix
	//============================================

	WriteShader();
	//Bind Shader
	mDefVS->SetShader();
	mDefPS->SetShader();
	if(mMaterial.tex)
		mDefPS->SetTexture(0, mMaterial.tex.get());
	mMesh->Draw();
}

void CanvasUI::SetCanvasSize(float width, float height)
{
	mTransform.SetScale(width, height, 1.0f);
	mOriginScale = { width,height };
}

void CanvasUI::SetPosZ(float z)
{
	mTransform.SetPositionZ(z);
}

void CanvasUI::SetPosition(float x, float y)
{
	mOriginPos.x = x;
	mOriginPos.y = y;
	mTransform.SetPosition(x, y);
}

json CanvasUI::SaveData(const char* objName)
{
	json data;
	data["Position"] = {GetPosition().x,GetPosition().y,GetPosition().z };
	data["Scale"] = { GetScale().x,GetScale().y,GetScale().z };
	data["Rotation"] = { GetRotation().x,GetRotation().y,GetRotation().z };
	data["Filepath"] = GetFilePath();

	//Set Material
	data["Material"]["Ambient"] = { GetMaterial().ambient.x,GetMaterial().ambient.y, GetMaterial().ambient.z, GetMaterial().ambient.w };
	data["Material"]["Diffuse"] = { GetMaterial().diffuse.x, GetMaterial().diffuse.y, GetMaterial().diffuse.z, GetMaterial().diffuse.w };
	data["Material"]["Specular"] = { GetMaterial().specular.x,GetMaterial().specular.y,GetMaterial().specular.z,GetMaterial().specular.w };
	data["Material"]["Emission"] = { GetMaterial().emission.x, GetMaterial().emission.y, GetMaterial().emission.z, GetMaterial().emission.w };

	return data;
}

void CanvasUI::SetColor(DirectX::XMFLOAT4 color)
{
	mMaterial.material.diffuse = color;
}

void CanvasUI::SetTransparency(float _transparency)
{
	mMaterial.material.diffuse.w = _transparency;
}

void CanvasUI::CreateMeshBuffer()
{

	const float d = 0.5f;
	DirectX::XMFLOAT4 color(1.0f, 1.0f, 1.0f, 1.0f);

	DirectX::XMFLOAT3 pos[] = {

		XMFLOAT3(-d, d, 0),	//left top
		XMFLOAT3(d, d, 0),	//right top
		XMFLOAT3(d, -d,0),	//right bot
		XMFLOAT3(-d, -d, 0)	//left bot
	};

	std::vector<Vertex::VtxPosNormalTex> vtx;
	vtx.resize(4);

	vtx = {
		//+y
		{pos[0],Vector3(0.0f,0.0f,-1.0f),Vector2(0.f,0.f)},
		{pos[1],Vector3(0.0f,0.0f,-1.0f),Vector2(1.f / mUvAnimation->GetSplit().x,0.f)},
		{pos[2],Vector3(0.0f,0.0f,-1.0f),Vector2(1.f / mUvAnimation->GetSplit().x,1.f / mUvAnimation->GetSplit().y)},
		{pos[3],Vector3(0.0f,0.0f,-1.0f),Vector2(0.f,1.f / mUvAnimation->GetSplit().y)},
	};

	std::vector<DWORD> indexData;
	indexData.resize(6);
	indexData = {
		0,1,2,2,3,0
	};
	Mesh::MeshData desc = {};
	desc.pVertex = vtx.data();
	desc.vertexSize = sizeof(Vertex::VtxPosNormalTex);
	desc.vertexCount = static_cast<UINT>(vtx.size());
	desc.pIndex = indexData.data();
	desc.indexSize = sizeof(DWORD);
	desc.indexCount = static_cast<UINT>(indexData.size());
	desc.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	mMesh = std::make_unique<Mesh>(desc);

}

void CanvasUI::CreateTexture(const char* _fileName)
{
	if(!_fileName)
	{
		DebugLog::LogWarning("File Path is wrong");
		mMaterial.material.isTexEnable = false;
		mMaterial.tex = nullptr;
		return;
	}

	mMaterial.tex = std::make_shared<Texture>();
	mMaterial.tex->Create(_fileName);
}

void CanvasUI::CreateTexture(std::shared_ptr<Texture> pTex)
{
	if(!pTex)
	{
		DebugLog::LogError("Texture pointer is null");
		mMaterial.material.isTexEnable = false;
		mMaterial.tex = nullptr;
		return;
	}
	mMaterial.tex = pTex;
}

void CanvasUI::CreateMaterial(const char* _fileName)
{
	mMaterial.material = {
		Color(1.0f, 1.0f, 1.0f, 1.0f),		// 環境光
		Color(1.0f, 1.0f, 1.0f, 1.0f),		// 表面色
		Color(1.0f, 0.5f, 0.5f, 0.2f),		// 鏡面反射: specular power 1
		Color(0.0f, 0.0f, 0.0f, 0.0f)		// 自発光なし};
	};
	CreateTexture(_fileName);
	if(_fileName)
		mFilePath = _fileName;
		

}

void CanvasUI::CreateMaterial(MaterialData& _materialData)
{
	mMaterial.material = _materialData.material;
	mMaterial.tex = _materialData.tex;
}

void CanvasUI::CreateMaterial(std::shared_ptr<Texture> tex)
{
	mMaterial.material = {};
	mMaterial.tex = tex;
}


void CanvasUI::LoadShader()
{
	mDefPS = std::make_shared<PixelShader>();
	mDefVS = std::make_shared<VertexShader>();

	HR(mDefPS->LoadShaderFile("Assets/Shader/PS_DefaultUI.cso"));
	HR(mDefVS->LoadShaderFile("Assets/Shader/VS_DefaultUI.cso"));

}

void CanvasUI::LoadPSShader(const char* filePath)
{
	mDefPS.reset();
	mDefPS = std::make_shared<PixelShader>();
	HR(mDefPS->LoadShaderFile(filePath));
}

void CanvasUI::SetShader(const char* PSFile, const char* VSFile)
{
	mDefPS.reset();
	mDefVS.reset();
	mDefPS = std::make_shared<PixelShader>();
	mDefVS = std::make_shared<VertexShader>();
	mDefPS->LoadShaderFile(PSFile);
	mDefVS->LoadShaderFile(VSFile);
}


void CanvasUI::UpdateScale()
{
	if (!gD3D->GetResized()) { return; }

	float viewWidth = static_cast<float>(gD3D->GetWinWidth());
	float viewHeight = static_cast<float>(gD3D->GetWinHeight());
	Vector3 ratio = { viewWidth / WIN_WIDTH,viewHeight / WIN_HEIGHT,1.0f };
	Vector3 scale = { ratio.x * mOriginScale.x,ratio.y * mOriginScale.y,1.0f };
	mTransform.SetScale(scale);
	
}

void CanvasUI::UpdatePos()
{
	if (!gD3D->GetResized()) { return; }
	float viewWidth = static_cast<float>(gD3D->GetWinWidth());
	float viewHeight = static_cast<float>(gD3D->GetWinHeight());
	Vector3 ratio = { viewWidth / WIN_WIDTH,viewHeight / WIN_HEIGHT,1.0f };
	Vector3 pos = {mOriginPos.x,mOriginPos.y, mTransform.GetPosition().z};
	pos *= ratio;
	mTransform.SetPosition(pos);
	

}

void CanvasUI::WriteShader()
{
	Vector2 viewSize = { static_cast<float>(gD3D->GetWinWidth()),static_cast<float>(gD3D->GetWinHeight()) };

	XMMATRIX WVP[3] = {};
	WVP[0] = mTransform.GetMatrix();
	WVP[1] = XMMatrixIdentity();
	WVP[2] = XMMatrixOrthographicLH(viewSize.x, viewSize.y, 0.0f, 3.0f);
	WVP[2] = XMMatrixTranspose(WVP[2]);

	struct UVBuffer
	{
		XMMATRIX uv;
		int useUV;
	};
	UVBuffer uvBuffer;
	uvBuffer.useUV = isUseUVAnimation;

	//UV MATRIX 作成
	if (isUseUVAnimation)
	{
		uvBuffer.uv = XMMatrixTranslation(mUvAnimation->GetOffsetUV().x, mUvAnimation->GetOffsetUV().y, 0.0f);
		uvBuffer.uv = XMMatrixTranspose(uvBuffer.uv);
	}

	//Write Shader
	mDefVS->WriteShader(0, WVP);
	mDefVS->WriteShader(1, &uvBuffer);
	mDefPS->WriteShader(0, &(mMaterial.material));
}


