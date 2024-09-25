#include "Water.h"
#include "DirLight.h"
#include "FirstPersonCamera.h"
#include "GampApp.h"
#include "GUI.h"
#include "KInput.h"
#include "Plane3D.h"
#include "RenderState.h"

using namespace DirectX::SimpleMath;
using namespace DirectX;

Water::Water()
{

}

void Water::Init(const char* filePath)
{
	mModel = std::make_unique<Circle>();
	mModel->CreateMesh(50, 50);
	mModel->CreateMaterial();
	mModel->CreateTexture(filePath);
	mModel->LoadDefShader("Assets/Shader/VS_Water.cso","Assets/Shader/PS_Water.cso");
	
}

void Water::Update(float dt)
{

#ifdef _DEBUG
	//…‚Ì”g‚ÌÝ’è‚ÆF‚ÌÝ’è‚ð‰ÂŽ‹‰»‚É‚·‚é
	if (ImGui::Begin("Water Option"))
	{
		static bool isResetMaterial = false;
		ImGui::Checkbox("Reset Material", &isResetMaterial);
		if (isResetMaterial)ResetMaterial();

		ImGui::Text("WaterPos");

		float pos[3] = { mModel->GetPosition().x,mModel->GetPosition().y,mModel->GetPosition().z };
		ImGui::InputFloat3("Position", pos);
		mModel->SetPosition(pos);

		float scale[3] = { mModel->GetScale().x,mModel->GetScale().y,mModel->GetScale().z };
		ImGui::InputFloat3("Scale", scale);
		mModel->SetScale(scale);
	

		ImGui::Text("Wave Option");
		float center[3] = { mParam.center.x,mParam.center.y,mParam.center.z };
		ImGui::SliderFloat3("Centre", center, -10.f, 10.f);
		mParam.center = Vector3(center);

		// ”g‚Ì•
		ImGui::SliderFloat("Amplitude", &mParam.amplitude, 0.f, 3.f);

		// ”g‚Ì•p“x
		ImGui::SliderFloat("Frequency", &mParam.frequency, 0.1f, 10.f);

		// ”g‚ÌƒXƒr[ƒh
		ImGui::SliderFloat("Speed", &mParam.speed, 0.1f, 10.f);

		// ”g‚ÌŠgŽU”ÍˆÍ
		ImGui::SliderFloat("Sigma", &mParam.sigma, 0.1f, 10.f);

		// Œp‘±ŽžŠÔ
		ImGui::SliderFloat("duration", &mDuration, 0.1f, 10.f);

		float ambient[4] = {
			GetModel()->GetMaterial().ambient.x, GetModel()->GetMaterial().ambient.y,
			GetModel()->GetMaterial().ambient.z, GetModel()->GetMaterial().ambient.w,
		};
		ImGui::ColorEdit4("Ambient", ambient);

		float diffuse[4] = { GetModel()->GetMaterial().diffuse.x, GetModel()->GetMaterial().diffuse.y, GetModel()->GetMaterial().diffuse.z, GetModel()->GetMaterial().diffuse.w };
		ImGui::ColorEdit4("Diffuse", diffuse);

		float specular[4] = { GetModel()->GetMaterial().specular.x,GetModel()->GetMaterial().specular.y,GetModel()->GetMaterial().specular.z,GetModel()->GetMaterial().specular.w };
		ImGui::ColorEdit4("Specular", specular);

		float emission[4] = { GetModel()->GetMaterial().emission.x, GetModel()->GetMaterial().emission.y, GetModel()->GetMaterial().emission.z, GetModel()->GetMaterial().emission.w };
		ImGui::ColorEdit4("Emission", emission);

		Material mat = {
		Color(ambient),
		Color(diffuse),
		Color(specular),
		Color(emission),
		};
		mModel->SetMaterial(mat);

	}

	ImGui::End();
#endif


	if(KInput::IsKeyTrigger(VK_SPACE))
	{
		isTrigger = true;
	}

	if (isTrigger)
	{
		if (mParam.time < mDuration)
		{
			// Update Time& amplitude
			mParam.time += dt;
			mParam.nowAmplitude = mParam.amplitude *(1.0f - mParam.time / mDuration);
		}
		else
		{
			//Rest Water state
			isTrigger = false; 
			mParam.nowAmplitude = 0.0f;
			mParam.time = 0;
		}
	}

	RenderUpdate();	

}

void Water::RenderUpdate()
{
	
	//‚±‚±‚Å”g‚É‚æ‚é—±Žq‚ÌŒvŽZ‚ðs‚¤
	for(auto& vertex:mModel->mVertices)
	{

		Vector2 lenVec = { vertex.pos.x - mParam.center.x,vertex.pos.z - mParam.center.z };
		float distanceToCenter = lenVec.Length();

		if (distanceToCenter < mParam.sigma)
		{
			float waveHeight = mParam.nowAmplitude * exp(-distanceToCenter * distanceToCenter / (2.f * mParam.sigma * mParam.sigma)) * sin(2.f * 3.14159f * (mParam.frequency * mParam.time - distanceToCenter / mParam.speed));

			vertex.pos.y = waveHeight;
		}
	}

	//XV‚µ‚½’¸“_ƒf[ƒ^‚ð‘‚«ž‚Ý
	mModel->mMesh->Write(mModel->mVertices.data());

	WriteShader();
}

void Water::Draw()
{

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

	mModel->GetDefPS()->WriteShader(0, &mModel->GetMaterial());
	mModel->GetDefPS()->WriteShader(1, &eyePos);
	mModel->GetDefPS()->WriteShader(2, &light);
}

void Water::SetCenter(DirectX::XMFLOAT3 centerPos)
{
	mParam.center = centerPos;
}

void Water::SetFrequency(float frequency)
{
	mParam.frequency = frequency;
}

void Water::SetSpeed(float speed)
{
	mParam.speed = speed;
}

void Water::SetAmplitude(float amplitude)
{
	mParam.amplitude = amplitude;
}

void Water::SetWaveDuration(float duration)
{
	mDuration = duration;
}

void Water::LoadSaveData(json data, const char* objName)
{
	mModel = std::make_unique<Circle>();

	//Init Model
	std::string filePath = data[objName]["Filepath"].get<std::string>();
	//CreateMesh
	mModel->CreateMesh(50, 50);
	//CreateTexture
	mModel->CreateTexture(filePath.c_str());
	//LoadShader
	mModel->LoadDefShader("Assets/Shader/VS_Water.cso", "Assets/Shader/PS_Water.cso");

	//Init Pos
	Vector3 pos = Vector3(data[objName]["Position"][0], data[objName]["Position"][1], data[objName]["Position"][2]);
	mModel->SetPosition(pos);

	//Init Rotation
	Vector3 rotation = Vector3(data[objName]["Rotation"][0], data[objName]["Rotation"][1], data[objName]["Rotation"][2]);
	mModel->mTransform.SetRotationInDegree(rotation);

	//Init Scale
	Vector3 scale = Vector3(data[objName]["Scale"][0], data[objName]["Scale"][1], data[objName]["Scale"][2]);
	mModel->SetScale({ scale.x, scale.z });

	//Init Material
	Material mat = {
		Color(data[objName]["Material"]["Ambient"][0],data[objName]["Material"]["Ambient"][1],data[objName]["Material"]["Ambient"][2],data[objName]["Material"]["Ambient"][3]),
		Color(data[objName]["Material"]["Diffuse"][0],data[objName]["Material"]["Diffuse"][1],data[objName]["Material"]["Diffuse"][2],data[objName]["Material"]["Diffuse"][3]),
		Color(data[objName]["Material"]["Specular"][0],data[objName]["Material"]["Specular"][1],data[objName]["Material"]["Specular"][2],data[objName]["Material"]["Specular"][3]),
		Color(data[objName]["Material"]["Emission"][0],data[objName]["Material"]["Emission"][1],data[objName]["Material"]["Emission"][2],data[objName]["Material"]["Emission"][3]),
	};
	mModel->SetMaterial(mat);

	//Set Water Param
	mParam.center = { data[objName]["Center"][0],data[objName]["Center"][1],data[objName]["Center"][2] };
	mParam.amplitude = data[objName]["Amplitude"][0];
	mParam.frequency = data[objName]["Frequency"][0];
	mParam.speed = data[objName]["Speed"][0];
	mParam.sigma = data[objName]["Sigma"][0];
	mParam.time = 0;

}

json Water::SaveData()
{
	json data;
	data["Position"] = {GetModel()->GetPosition().x,GetModel()->GetPosition().y,GetModel()->GetPosition().z };
	data["Scale"] = { GetModel()->GetScale().x,GetModel()->GetScale().y,GetModel()->GetScale().z };
	data["Rotation"] = { GetModel()->GetRotation().x,GetModel()->GetRotation().y,GetModel()->GetRotation().z };
	data["Filepath"] = GetModel()->GetFilePath();

	//Set Material
	data["Material"]["Ambient"] = { GetModel()->GetMaterial().ambient.x,GetModel()->GetMaterial().ambient.y, GetModel()->GetMaterial().ambient.z, GetModel()->GetMaterial().ambient.w };
	data["Material"]["Diffuse"] = { GetModel()->GetMaterial().diffuse.x, GetModel()->GetMaterial().diffuse.y, GetModel()->GetMaterial().diffuse.z, GetModel()->GetMaterial().diffuse.w };
	data["Material"]["Specular"] = { GetModel()->GetMaterial().specular.x,GetModel()->GetMaterial().specular.y,GetModel()->GetMaterial().specular.z,GetModel()->GetMaterial().specular.w };
	data["Material"]["Emission"] = { GetModel()->GetMaterial().emission.x, GetModel()->GetMaterial().emission.y, GetModel()->GetMaterial().emission.z, GetModel()->GetMaterial().emission.w };

	//Set Water Param
	data["Center"] = { mParam.center.x,mParam.center.y,mParam.center.z };
	data["Amplitude"] = { mParam.amplitude };
	data["Frequency"] = { mParam.frequency };
	data["Speed"] = { mParam.speed };
	data["Sigma"] = { mParam.sigma };


	return data;
}

void Water::ResetMaterial()
{
	mModel->SetMaterial(defaultMat);
}

void Water::LateUpdate(float dt)
{

}


