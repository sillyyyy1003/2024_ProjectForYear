#include "Water.h"
#include "DirLight.h"
#include "FirstPersonCamera.h"
#include "GameApp.h"
#include "KInput.h"
#include "Square.h"
#include "RenderState.h"

using namespace DirectX::SimpleMath;
using namespace DirectX;

XMFLOAT3 CalculateSphericalNormal(float r, float theta, float phi, float dTheta, float dPhi)
{
	float nx = r * cos(phi) * cos(theta + dTheta) - r * cos(phi) * cos(theta - dTheta);
	float ny = r * sin(phi + dPhi) - r * sin(phi - dPhi);
	float nz = r * cos(phi + dPhi) * sin(theta) - r * cos(phi - dPhi) * sin(theta);

	return XMFLOAT3(nx, ny, nz);
}


Water::Water()
{

}

void Water::Init(const char* filePath, const char* objName, int slices)
{
	mModel = std::make_unique<Circle>();
	mModel->Init(filePath, slices, slices,{1,1});
	mModel->LoadDefShader();

	mObjectName = objName;

	mWaterStates = std::make_unique<WaterState>();
	mWaterStates->Init();
}

void Water::Init(const std::shared_ptr<Texture>& tex, const char* objName, int slices)
{
	mModel = std::make_unique<Circle>();
	mModel->Init(tex, slices, slices, { 1,1 });
	mModel->LoadDefShader();
	mObjectName = objName;

	mWaterStates = std::make_unique<WaterState>();
	mWaterStates->Init();
}

void Water::Update(float dt)
{

#ifdef _DEBUG
	//水の波の設定と色の設定を可視化にする
	mWaterStates->DebugFunction();

	if (ImGui::Begin("Water Option"))
	{
		static bool isResetMaterial = false;
		ImGui::Checkbox("Reset Material", &isResetMaterial);
		if (isResetMaterial)ResetMaterial();

		ImGui::Checkbox("Apply param", &isApplyParam);
		if(isApplyParam)mParam = mWaterStates->GetCurrentWaterParam();

		
		const char* stateName[] = {"still","boiling","experiment"};
		int currentState =static_cast<int>(mBoilingState);
		// 下拉菜单选择状态
		if (ImGui::Combo("Select Water State", &currentState, stateName, IM_ARRAYSIZE(stateName)))
		{
			mBoilingState = static_cast<WaterStateConfig::WaterBoilingState>(currentState);
		}

		ImGui::Text("WaterPos");

		float pos[3] = { mModel->GetPosition().x,mModel->GetPosition().y,mModel->GetPosition().z };
		ImGui::InputFloat3("Position", pos);
		mModel->SetPosition(pos);

		float scale[3] = { mModel->GetScale().x,mModel->GetScale().y,mModel->GetScale().z };
		ImGui::InputFloat3("Scale", scale);
		mModel->SetScale(scale);

		// 継続時間
		ImGui::SliderFloat("duration", &mDuration, 0.1f, 10.f);
		
		float ambient[4] = {
			mModel->GetMaterial().ambient.x, mModel->GetMaterial().ambient.y,
			mModel->GetMaterial().ambient.z, mModel->GetMaterial().ambient.w,
		};
		ImGui::ColorEdit4("Ambient", ambient);

		float diffuse[4] = { mModel->GetMaterial().diffuse.x, mModel->GetMaterial().diffuse.y, mModel->GetMaterial().diffuse.z, mModel->GetMaterial().diffuse.w };
		ImGui::ColorEdit4("Diffuse", diffuse);

		float specular[4] = { mModel->GetMaterial().specular.x,mModel->GetMaterial().specular.y,mModel->GetMaterial().specular.z,mModel->GetMaterial().specular.w };
		ImGui::ColorEdit4("Specular", specular);

		float emission[4] = { mModel->GetMaterial().emission.x, mModel->GetMaterial().emission.y, mModel->GetMaterial().emission.z, mModel->GetMaterial().emission.w };
		ImGui::ColorEdit4("Emission", emission);

		float isTexEnable = mModel->GetMaterial().isTexEnable;
		Material mat = {
		Color(ambient),
		Color(diffuse),
		Color(specular),
		Color(emission),
			isTexEnable,
			0,0,0
		};
		mModel->SetMaterial(mat);

	}

	ImGui::End();

	if (KInput::IsKeyTrigger(VK_RETURN))
	{
		mBoilingState = WaterStateConfig::WaterBoilingState::STATE_STILL;
		isTrigger = true;
	}

#endif




	switch(mBoilingState)
	{
	default:
#ifdef _DEBUG
	case WaterStateConfig::WaterBoilingState::STATE_EXPERIMENT:
		break;
#endif
	case WaterStateConfig::WaterBoilingState::STATE_STILL:
		mParam = mWaterStates->GetCurrentWaterParam();
		if (KInput::IsKeyTrigger(VK_SPACE))
		{
			isTrigger = true;
		}

		if (isTrigger)
		{
			if (mWaterTime < mDuration)
			{
				mWaterTime += dt;
				mNowAmplitude = (mParam.maxAmplitude - mParam.minAmplitude) * (1.0f - mWaterTime / mDuration);
			}
			else
			{
				// 恢复为平静状态
				isTrigger = false;
				mNowAmplitude = mParam.minAmplitude;
				mWaterTime = 0;
			}
			isResetVertices = false;
		}
		break;
	case WaterStateConfig::WaterBoilingState::STATE_BOILING:
		mParam = mWaterStates->GetCurrentWaterParam();
		mWaterTime += dt;
		mNowAmplitude = mParam.maxAmplitude;
		break;
	}

	RenderUpdate();	
}

void Water::RenderUpdate()
{

	switch(mBoilingState)
	{
	default:
	case WaterStateConfig::WaterBoilingState::STATE_STILL:

		//頂点のリセットが完了した場合
		if (isResetVertices)break;

		//リセットする場合
		if (mNowAmplitude == 0)
		{
			for (auto& vertex : mModel->mVertices)
			{
				vertex.pos.y = 0.0f;
			}
			isResetVertices = true;

			RewriteVertices();
			break;
		}

		// 一般の場合
		for (auto& vertex : mModel->mVertices)
		{
			//Transform World to Local
			Vector2 ripplePos = { mParam.center.x,mParam.center.y };
			ripplePos -= Vector2(mModel->GetPosition().x, mModel->GetPosition().z);
			ripplePos = Vector2(ripplePos.x / mModel->GetScale().x, ripplePos.y / mModel->GetScale().z);

			Vector2 lenVec = { vertex.pos.x - ripplePos.x,vertex.pos.z - ripplePos.y };
			float sigma = mParam.sigma / mModel->GetScale().x;
			float speed = mParam.speed / mModel->GetScale().x;
			float distanceToCenter = lenVec.Length();
			if (distanceToCenter < mParam.sigma / mModel->GetScale().x)
			{
				float waveHeight = mNowAmplitude * exp(-distanceToCenter * distanceToCenter / (2.f * sigma * sigma)) * sin(2.f * 3.14159f * (mParam.frequency * mWaterTime - distanceToCenter / speed));
				vertex.pos.y = waveHeight;
			}
		}
		/*
		for (size_t i = 1; i < WaterDefault::DEFAULT_SLICES - 1; ++i)
		{
			for (size_t j = 1; j < WaterDefault::DEFAULT_SLICES - 1; ++j)
			{
				// 获取相邻点的球面坐标，假设这些点存储在经纬度网格中
				float r = mModel->mVertices[i * WaterDefault::DEFAULT_SLICES + j].pos.x;
				float theta = mModel->mVertices[i * WaterDefault::DEFAULT_SLICES + j].pos.y;
				float phi = mModel->mVertices[i * WaterDefault::DEFAULT_SLICES + j].pos.z;

				// 计算相邻点的经纬度差异
				float dTheta = mModel->mVertices[(i + 1) * WaterDefault::DEFAULT_SLICES + j].pos.y - mModel->mVertices[(i - 1) * WaterDefault::DEFAULT_SLICES + j].pos.y;
				float dPhi = mModel->mVertices[i * WaterDefault::DEFAULT_SLICES + (j + 1)].pos.z - mModel->mVertices[i * WaterDefault::DEFAULT_SLICES + (j - 1)].pos.z;

				// 基于球面坐标的法向量计算
				XMFLOAT3 normal = CalculateSphericalNormal(r, theta, phi, dTheta, dPhi);

				// 归一化法向量
				XMVECTOR nVec = XMVector3Normalize(XMLoadFloat3(&normal));
				XMStoreFloat3(&mModel->mVertices[i * WaterDefault::DEFAULT_SLICES + j].normal, nVec);
			}
		}
		*/
		//頂点データに再書き込み
		RewriteVertices();
		break;

	case WaterStateConfig::WaterBoilingState::STATE_BOILING:

		//ここで波による粒子の計算を行う
		for (auto& vertex : mModel->mVertices)
		{
			//Transform World to Local
			Vector2 ripplePos = { mParam.center.x,mParam.center.y };
			ripplePos -= Vector2(mModel->GetPosition().x, mModel->GetPosition().z);
			ripplePos = Vector2(ripplePos.x / mModel->GetScale().x, ripplePos.y / mModel->GetScale().z);

			Vector2 lenVec = { vertex.pos.x - ripplePos.x,vertex.pos.z - ripplePos.y };
			float sigma = mParam.sigma / mModel->GetScale().x;
			float speed = mParam.speed / mModel->GetScale().x;
			float distanceToCenter = lenVec.Length();
			if (distanceToCenter < mParam.sigma / mModel->GetScale().x)
			{
				float waveHeight = mNowAmplitude * exp(-distanceToCenter * distanceToCenter / (2.f * sigma * sigma)) * sin(2.f * 3.14159f * (mParam.frequency * mWaterTime - distanceToCenter / speed));
				vertex.pos.y = waveHeight;
			

			}
		}
		for (size_t i = 1; i < WaterDefault::DEFAULT_SLICES - 1; ++i)
		{
			for (size_t j = 1; j < WaterDefault::DEFAULT_SLICES - 1; ++j)
			{
				// 当前点位置
				XMFLOAT3 current = mModel->mVertices[i * WaterDefault::DEFAULT_SLICES + j].pos;

				// 获取相邻点的顶点位置
				XMFLOAT3 left = mModel->mVertices[i * WaterDefault::DEFAULT_SLICES + (j - 1)].pos;
				XMFLOAT3 right = mModel->mVertices[i * WaterDefault::DEFAULT_SLICES + (j + 1)].pos;
				XMFLOAT3 top = mModel->mVertices[(i - 1) * WaterDefault::DEFAULT_SLICES + j].pos;
				XMFLOAT3 bottom = mModel->mVertices[(i + 1) * WaterDefault::DEFAULT_SLICES + j].pos;

				// 计算相邻点的方向向量
				XMVECTOR vec1 = XMVectorSubtract(XMLoadFloat3(&right), XMLoadFloat3(&left));
				XMVECTOR vec2 = XMVectorSubtract(XMLoadFloat3(&bottom), XMLoadFloat3(&top));

				// 计算叉积以获得法向量
				XMVECTOR normal = XMVector3Cross(vec1, vec2);

				// 归一化法向量
				normal = XMVector3Normalize(normal);

				// 存储法向量
				XMStoreFloat3(&mModel->mVertices[i * WaterDefault::DEFAULT_SLICES + j].normal, normal);
			}
		}

		//頂点データに再書き込み
		RewriteVertices();
		break;
	}

}

void Water::Draw()
{
	WriteShader();
	mModel->Draw();

}

void Water::WriteShader()
{
	//fail safe
	if (!mModel->GetVS() || !mModel->GetPS())
	{
		DebugLog::LogWarning("VertexShader/PixelShader is Null!");
		return;
	}
	
	CameraBase* firstCamera = GameApp::GetCurrentCamera();
	std::shared_ptr<DirLight> dirLight = SceneManager::Get()->GetObj<DirLight>("EnvironmentLight");

	XMFLOAT4X4 WVP[3] = {};
	//WORLD
	WVP[0] = mModel->mTransform.GetMatrixFX4();

	//VIEW
	WVP[1] = firstCamera->GetViewXMF();

	//PROJ
	WVP[2] = firstCamera->GetProjXMF();

	XMFLOAT4 eyePos = { firstCamera->GetPos().x,firstCamera->GetPos().y ,firstCamera->GetPos().z ,0.0f };

	NormalConstantBuffer cb = {

			eyePos,
			Vector4{dirLight->GetAmbient().x,dirLight->GetAmbient().y,dirLight->GetAmbient().z,dirLight->GetAmbient().w},
		   Vector4{ dirLight->GetDiffuse().x,dirLight->GetDiffuse().y,dirLight->GetDiffuse().z,dirLight->GetDiffuse().w},
		Vector4{dirLight->GetPosition().x,dirLight->GetPosition().y,dirLight->GetPosition().z,0},
		mModel->GetMaterial()
	};


	mModel->GetVS()->WriteShader(0, WVP);
	mModel->GetPS()->WriteShader(0, &cb);
}

void Water::LoadDefShader(const std::shared_ptr<VertexShader>& vs, const std::shared_ptr<PixelShader>& ps)
{
	mModel->LoadDefShader(vs,ps);
}

void Water::LoadShader(const std::shared_ptr<VertexShader>& vs, const std::shared_ptr<PixelShader>& ps)
{
	mModel->SetVertexShader(vs.get());
	mModel->SetPixelShader(ps.get());
}

void Water::LoadDefShader(const char* vsPath, const char* psPath)
{
	mModel->LoadDefShader(vsPath, psPath);
}

void Water::SetWaterState(WaterStateConfig::WaterState _state)
{
	mWaterStates->SetCurrentWaterParam(_state);
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
	mParam.maxAmplitude = amplitude;
}

void Water::SetWaveDuration(float duration)
{
	mDuration = duration;
}

void Water::LoadSaveData(json data, const char* objName)
{
	Init(nullptr, objName);
	//Init Pos
	Vector3 pos = Vector3(data[objName]["Position"][0], data[objName]["Position"][1], data[objName]["Position"][2]);
	mModel->SetPosition(pos);

	//Init Rotation
	Vector3 rotation = Vector3(data[objName]["Rotation"][0], data[objName]["Rotation"][1], data[objName]["Rotation"][2]);
	mModel->mTransform.SetRotationInDegree(rotation);

	//Init Scale
	Vector3 scale = Vector3(data[objName]["Scale"][0], data[objName]["Scale"][1], data[objName]["Scale"][2]);
	mModel->SetScaleXZ({ scale.x, scale.z });

	//Init Material
	Material mat = {
		Color(data[objName]["Material"]["Ambient"][0],data[objName]["Material"]["Ambient"][1],data[objName]["Material"]["Ambient"][2],data[objName]["Material"]["Ambient"][3]),
		Color(data[objName]["Material"]["Diffuse"][0],data[objName]["Material"]["Diffuse"][1],data[objName]["Material"]["Diffuse"][2],data[objName]["Material"]["Diffuse"][3]),
		Color(data[objName]["Material"]["Specular"][0],data[objName]["Material"]["Specular"][1],data[objName]["Material"]["Specular"][2],data[objName]["Material"]["Specular"][3]),
		Color(data[objName]["Material"]["Emission"][0],data[objName]["Material"]["Emission"][1],data[objName]["Material"]["Emission"][2],data[objName]["Material"]["Emission"][3]),
	};
	mModel->SetMaterial(mat);

	mDuration = data[objName]["Duration"][0];

	//Set Water Param
	mWaterStates->LoadWaterStateData(data[objName]["WaterParam"]);
}

void Water::UpdateColor(DirectX::XMFLOAT4 color)
{
	Vector4 waterColor = mModel->GetMaterial().diffuse;
	waterColor += color;
	mModel->SetDiffuse(waterColor);
}

json Water::SaveData()
{
	json data;
	
	data["Position"] = {mModel->GetPosition().x,mModel->GetPosition().y,mModel->GetPosition().z };
	data["Scale"] = { mModel->GetScale().x,mModel->GetScale().y,mModel->GetScale().z };
	data["Rotation"] = { mModel->GetRotation().x,mModel->GetRotation().y,mModel->GetRotation().z };

	//Set Material
	data["Material"]["Ambient"] = { mModel->GetMaterial().ambient.x,mModel->GetMaterial().ambient.y, mModel->GetMaterial().ambient.z, mModel->GetMaterial().ambient.w };
	data["Material"]["Diffuse"] = { mModel->GetMaterial().diffuse.x, mModel->GetMaterial().diffuse.y, mModel->GetMaterial().diffuse.z, mModel->GetMaterial().diffuse.w };
	data["Material"]["Specular"] = { mModel->GetMaterial().specular.x,mModel->GetMaterial().specular.y,mModel->GetMaterial().specular.z,mModel->GetMaterial().specular.w };
	data["Material"]["Emission"] = { mModel->GetMaterial().emission.x, mModel->GetMaterial().emission.y, mModel->GetMaterial().emission.z, mModel->GetMaterial().emission.w };

	//WaterDuration
	data["Duration"] = {mDuration};


	//Save water param
	data["WaterParam"] = mWaterStates->SaveWaterParam();
	return data;
}

void Water::SetTexture(std::shared_ptr<Texture> tex)
{
	mModel->LoadTexture(tex);
}

void Water::RewriteVertices()
{
	//更新した頂点データを書き込み
	mModel->GetMesh()->Write(mModel->mVertices.data());
}

void Water::SetWaterBoilingState(WaterStateConfig::WaterBoilingState _state)
{
	mBoilingState = _state;
}


void Water::ResetMaterial()
{
	Material mat = WaterDefault::defaultMat;
	mat.isTexEnable = mModel->GetMaterial().isTexEnable;
	mModel->SetMaterial(mat);
}

void Water::LateUpdate(float dt)
{

}


