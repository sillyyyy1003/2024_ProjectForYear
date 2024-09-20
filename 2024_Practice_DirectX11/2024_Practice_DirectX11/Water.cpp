#include "Water.h"

#include "DirLight.h"
#include "FirstPersonCamera.h"
#include "GampApp.h"
#include "GUI.h"
#include "KInput.h"
#include "Plane3D.h"

using namespace DirectX::SimpleMath;
using namespace DirectX;

Water::Water()
{
}

void Water::Init(const char* filePath)
{
	//mModel = std::make_unique<Plane3D>();
	mModel = std::make_unique<Circle>();

	//mModel->CreateMesh(100);
	mModel->CreateMesh(50, 50);
	mModel->CreateMaterial();
	mModel->CreateTexture(filePath);
	mModel->LoadDefShader("Assets/Shader/VS_Water.cso","Assets/Shader/PS_Water.cso");

	mParam = {
		Vector3(0,0,0),
		0.05f,
		1.0f,
		2.0f,
		3.0f,
		0
	};


	Material mat = {
		Color(0.5f, 0.5f, 0.5f, 1.0f),
		Color(1.0f, 1.0f, 1.0f, 0.5f),
		Color(0.8f, 0.8f, 0.8f, 32.0f),
		 Color(0.0f, 0.0f, 0.0f, 0.0f)
	};

	mModel->SetMaterial(mat);
}

void Water::Update(float dt)
{
	if(KInput::IsKeyTrigger(VK_SPACE))
	{
		isTrigger = true;
	}

	if (isTrigger)
	{
		if (mParam.time < duration)
		{
			// Update Time& amplitude
			mParam.time += dt;
			mParam.amplitude = 0.05f * (1.0f - mParam.time / duration);
		}
		else
		{
			//Rest Water state
			isTrigger = false; 
			mParam.amplitude = 0.0f;
			mParam.time = 0;
		}
	}



}

void Water::Draw()
{
	WriteShader();
	mModel->Draw();
}

void Water::WriteShader()
{

	FirstPersonCamera* firstCamera = GameApp::GetComponent<FirstPersonCamera>("Camera");
	DirLight* dirLight = GameApp::GetComponent<DirLight>("Light");

	XMFLOAT4X4 WVP[3] = {};
	//WORLD
	WVP[0] = mModel->mTransform.GetMatrixFX4();

	//VIEW
	WVP[1] = firstCamera->GetViewXMF();

	//PROJ
	WVP[2] = firstCamera->GetProjXMF();

	XMFLOAT4 eyePos = { firstCamera->GetPos().x,firstCamera->GetPos().y ,firstCamera->GetPos().z ,0.0f };

	struct Light
	{
		XMFLOAT4 lightAmbient;
		XMFLOAT4 lightDiffuse;
		XMFLOAT4 lightDir;
	};

	Light light = {
		dirLight->GetAmbient(),
		dirLight->GetDiffuse(),
		Vector4{dirLight->GetPos().x,dirLight->GetPos().y,dirLight->GetPos().z,0},
	};

	mModel->GetDefVS()->WriteShader(0, WVP);
	mModel->GetDefVS()->WriteShader(1, &mParam);

	mModel->GetDefPS()->WriteShader(0, &mModel->GetMaterial());
	mModel->GetDefPS()->WriteShader(1, &eyePos);
	mModel->GetDefPS()->WriteShader(2, &light);


}


