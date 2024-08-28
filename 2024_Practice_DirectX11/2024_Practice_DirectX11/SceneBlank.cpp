#include "SceneBlank.h"
#include <memory>
#include "Model.h"
#include "Box3D.h"
#include "CanvasUI.h"
#include "DirLight.h"
#include "Plane3D.h"
#include "FirstPersonCamera.h"
#include "GUI.h"
#include "Shader.h"

using namespace DirectX::SimpleMath;
using namespace DirectX;

void SceneBlank::Init()
{
	
	VertexShader* vs = CreateObj<VertexShader>("vs");
	HR(vs->LoadShaderFile("Assets/Shader/VS_Box.cso"));
	
	PixelShader* ps = CreateObj<PixelShader>("ps");
	HR(ps->LoadShaderFile("Assets/Shader/PS_Box.cso"));


	for (int i = 0; i < 3; i++)
	{
		bg[i] = std::make_unique<Plane3D>();
		bg[i]->InitResource("Assets/Texture/wall000.jpg");
		bg[i]->mTransform.SetScale(1, 1.0, 1);

	}

	bg[0]->mTransform.SetPosition(-2, 0, 0);
	bg[0]->mTransform.SetRotationInDegree(-90, 0, 0);

	bg[1]->mTransform.SetPosition(0, -1, 0);

	bg[2]->mTransform.SetPosition(2, -2, 1);
	bg[2]->mTransform.SetRotationInDegree(0, 0, -90);


	model = std::make_unique<Model>();
	model->Load("Assets/Model/Player.obj");
	model->mTransform.SetPosition(1.0f, 1.0f, 0.0f);

	ui = std::make_unique<CanvasUI>();
	ui->InitCanvas("Assets/Texture/wall000.jpg");
	
}

void SceneBlank::UnInit()
{
}

void SceneBlank::Update(float dt)
{
	static float r = 0.0f;
	r += 1.f * dt ;
	
	//Box3D* box = GetObj<Box3D>("box");
	//model->mTransform.RotateAround(Vector3(0, 0, 0), Vector3(0, 0, 1), r);

	ui->Update(dt);

}

void SceneBlank::Draw()
{
	
	FirstPersonCamera* firstCamera = GetObj<FirstPersonCamera>("Camera");
	VertexShader* vs = GetObj<VertexShader>("vs");
	PixelShader* ps = GetObj<PixelShader>("ps");

	Box3D* box = GetObj<Box3D>("box");
	Material material = {
		Color(0.4f, 0.4f, 0.4f, 1.0f),		// ŠÂ‹«Œõ
		Color(0.7f, 0.3f, 0.5f, 1.0f),		// •\–ÊF
		Color(1.0f, 0.5f, 0.5f, 0.2f),		// ‹¾–Ê”½ŽË: specular power 1
		Color(0.0f, 0.0f, 0.0f, 0.0f)		// Ž©”­Œõ‚È‚µ};
	};
	//box->Draw();

	Model* model = GetObj<Model>("Model");
	XMFLOAT4X4 worldMat = model->mTransform.GetMatrixFX4();
	XMFLOAT4 eyePos = { firstCamera->GetPos().x,firstCamera->GetPos().y ,firstCamera->GetPos().z ,0.0f };

	XMFLOAT4X4 cameraMat[2]={};
	cameraMat[0] = firstCamera->GetViewXMF();
	cameraMat[1] = firstCamera->GetProjXMF();
	

	struct Light
	{
		DirectX::XMFLOAT4 lightAmbient;
		DirectX::XMFLOAT4 lightDiffuse;
		DirectX::XMFLOAT4 lightDir;
	};
	DirLight* dirLight = GetObj<DirLight>("Light");

	Light light = {
		dirLight->GetAmbient(),
		dirLight->GetDiffuse(),
		Vector4{0,0,-1,0},
	};


	vs->WriteShader(0, &worldMat);
	vs->WriteShader(1, cameraMat);
	ps->WriteShader(1, &eyePos);
	ps->WriteShader(2, &light);
	model->SetPixelShader(ps);
	model->SetVertexShader(vs);
	model->Draw();

	for (int i = 0; i < 3; i++)
	{
		XMFLOAT4X4 bgMat = bg[i]->mTransform.GetMatrixFX4();
		vs->WriteShader(0, &bgMat);
		vs->WriteShader(1, cameraMat);
		ps->WriteShader(0, &material);
		ps->WriteShader(1, &eyePos);
		ps->WriteShader(2, &light);
		bg[i]->SetPixelShader(ps);
		bg[i]->SetVertexShader(vs);
		bg[i]->Draw();
	}
	
	ui->Draw();
}
