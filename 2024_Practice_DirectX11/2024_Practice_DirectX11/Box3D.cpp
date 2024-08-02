#include "Box3D.h"

#include <iostream>

#include "D3DApp.h"
#include "DebugLog.h"
#include "Geometry.h"
#include "KInput.h"
using namespace DirectX;
using namespace DirectX::SimpleMath;

void Box3D::InitResource()
{
	CreatePS();
	CreateVS();
	CreateMaterial();
	CreateMeshes();
	CreateCamera();
	CreateLights(); //Only Use Ambient Light
}

void Box3D::SetMaterial(Material _material, int materialID)
{
	mMaterials[materialID] = _material;
}

void Box3D::Draw()
{
	GenerateMatrix();
	//todo:这一部分可以独立出来，这样可以对应不同的Shader

	for (auto it = mMeshes.begin(); it != mMeshes.end(); ++it)
	{
		mVS->WriteShader(0, mWVPMatrix);//WVP
		mPS->WriteShader(0, &mMaterials[0]);//Material
		mPS->WriteShader(1, &mCamera);
		mPS->WriteShader(2, &mLight);//Light
		mPS->SetShader();
		mVS->SetShader();
		it->mesh->Draw();
	}

}

void Box3D::CreateMaterial(int matNum)
{
	mMaterials.resize(matNum);

	for (int i = 0; i < mMaterials.size(); i++)
	{
		mMaterials[i] = {
			Color(0.2f, 0.2f, 0.2f, 1.0f),		// 弱い環境光
			Color(0.7f, 0.3f, 0.5f, 1.0f),		// 表面色
			Color(1.0f, 0.5f, 0.5f, 0.2f),		// 鏡面反射: specular power 1
			Color(0.0f, 0.0f, 0.0f, 0.0f)		// 自発光なし};
		};
	}
}

void Box3D::CreateMeshes()
{
	
	mMeshes.resize(6);
	const float d = 0.5f;
	DirectX::XMFLOAT4 color(1.0f, 1.0f, 1.0f, 1.0f);

	DirectX::XMFLOAT3 faceNormal[] = {
		DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f),
		DirectX::XMFLOAT3(-1.0f, 0.0f, 0.0f),
		DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f),
		DirectX::XMFLOAT3(0.0f,-1.0f, 0.0f),
		DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f),
		DirectX::XMFLOAT3(0.0f, 0.0f,-1.0f),
	};

	DirectX::XMFLOAT3 pos[] = {
		//+X面
		XMFLOAT3(d, -d, -d),
		XMFLOAT3(d, d, -d),
		XMFLOAT3(d, d, d),
		XMFLOAT3(d, -d, d),
		// -X面
		XMFLOAT3(-d, -d, d),
		XMFLOAT3(-d, d, d),
		XMFLOAT3(-d, d, -d),
		XMFLOAT3(-d, -d, -d),
		// +Y面
		XMFLOAT3(-d, d, -d),
		XMFLOAT3(-d, d, d),
		XMFLOAT3(d, d, d),
		XMFLOAT3(d, d, -d),
		// -Y面
		XMFLOAT3(d, -d, -d),
		XMFLOAT3(d, -d, d),
		XMFLOAT3(-d, -d, d),
		XMFLOAT3(-d, -d, -d),
		// +Z面
		XMFLOAT3(d, -d, d),
		XMFLOAT3(d, d, d),
		XMFLOAT3(-d, d, d),
		XMFLOAT3(-d, -d, d),
		// -Z面
		XMFLOAT3(-d, -d, -d),
		XMFLOAT3(-d, d, -d),
		XMFLOAT3(d, d, -d),
		XMFLOAT3(d, -d, -d),
	};

	//std::vector<XMFLOAT3>vtxNormals = CalculateVtxNormal(pos, faceNormal);

	mVtxData.resize(24);
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			mVtxData[i * 4 + j] = { pos[i * 4 + j], color, faceNormal[i] };
		}
	}

	for (int i = 0; i < 6; ++i)
	{
		std::vector<Vertex::VtxPosColorNormal> vertexData;
		std::vector<DWORD> indexData;
		// 每个面有4个顶点
		vertexData.resize(4);
		indexData.resize(6);

		for (int j = 0; j < 4; j++)
		{
			vertexData[j] = mVtxData[i * 4 + j];

		}

		indexData = {
			0,1,2,2,3,0
		};


		Mesh::MeshData desc = {};
		desc.pVertex = vertexData.data();
		desc.vertexSize = sizeof(Vertex::VtxPosColorNormal);
		desc.vertexCount = static_cast<UINT>(vertexData.size());
		desc.pIndex = indexData.data();
		desc.indexSize = sizeof(DWORD);
		desc.indexCount = static_cast<UINT>(indexData.size());
		desc.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		mMeshes[i].mesh = std::make_shared<Mesh>(desc);
		mMeshes[i].materialID = i;
	}
}

void Box3D::CreateVS()
{
	mVS = std::make_shared<VertexShader>(ShaderEnum::Vertex);
	HR(mVS->LoadShaderFile("Assets/Shader/VS_Box.cso"));
}

void Box3D::CreatePS()
{
	mPS = std::make_shared<PixelShader>(ShaderEnum::Pixel);
	HR(mPS->LoadShaderFile("Assets/Shader/PS_Box.cso"));
}

void Box3D::CreateLights()
{
	/*
	Lights lights = {
		// Directional Light
		{
			DirectX::XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f),  // ambient
			DirectX::XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f),  // diffuse
			DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f),  // specular
			DirectX::XMFLOAT3(-1.0f, -1.0f, 1.0f),		// direction
			0.0f												// isEnable
		},
		// Point Light
		{
			DirectX::XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f),  // ambient
			DirectX::XMFLOAT4(0.6f, 0.6f, 0.6f, 1.0f),  // diffuse
			DirectX::XMFLOAT4(0.7f, 0.7f, 0.7f, 1.0f),  // specular
			DirectX::XMFLOAT3(0.0f, 7.0f, -7.0f),			// position
			10.0f,												// range
			DirectX::XMFLOAT3(0.0f, 0.1f, 0.0f),        // attenuation
			1.0f												// isEnable
		},
		// Spotlight
	{
			DirectX::XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f),  // ambient
			DirectX::XMFLOAT4(0.7f, 0.7f, 0.7f, 1.0f),  // diffuse
			DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),  // specular
			DirectX::XMFLOAT3(0.0f, 5.0f, 0.0f),        // position
			20.0f,                                      // range
			DirectX::XMFLOAT3(0.0f, -1.0f, 0.0f),       // direction
			30.0f,                                      // Spot
			DirectX::XMFLOAT3(1.0f, 0.1f, 0.0f),        // attenuation
			0.0f                                        // isEnable
		}
	};

	mLights = lights;
	*/

	mLight = {
		Color{0.2f,0.2f,0.2f,1.0f},
		Color{0.2f,0.4f,0.6f,1.0f},
		Vector4{0.5,0.5,-5.0f,1.0f},
	};
}

void Box3D::CreateCamera()
{
	mCamera = { 0,0,-5.0f,0.0 };
}

void Box3D::GenerateMatrix()
{
	//World
	SetWorldMatrix(mTransform.GetMatrix());

	//View
	Matrix view = XMMatrixTranspose(XMMatrixLookAtLH(
		XMVectorSet(mCamera.x, mCamera.y,  mCamera.z,  0.0f),
		XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),
		XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
	));
	SetViewMatrix(view);

	//Projection
	Matrix proj = XMMatrixTranspose(XMMatrixPerspectiveFovLH(XM_PIDIV2, gD3D->AspectRatio(), 0.01f, 1000.0f));
	SetProjMatrix(proj);
}

std::vector<XMFLOAT3> Box3D::CalculateVtxNormal(DirectX::XMFLOAT3* pos, DirectX::XMFLOAT3* faceNormals)
{
	std::vector<XMFLOAT3> faceNormal(24);

	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			faceNormal[i * 4 + j] = faceNormals[i];
		}
	}

	std::vector<Vector3> vecPos(24);

	for (int i = 0; i < 24; i++)
	{
		vecPos[i] = pos[i];
	}

	std::vector<std::vector<Vector3>> countNormal(24);
	for (int i = 0; i < 24; i++)
	{
		for (int j = 1; j < 24; j++)
		{
			XMFLOAT3 findElement = vecPos[i];

			if(vecPos[j]==findElement)
			{
				countNormal[i].push_back(faceNormal[j]);
			}
		};
	}

	std::vector<XMFLOAT3> vtxNormal(24);
	for (int i = 0; i < 24; i++)
	{
		for (auto it = countNormal[i].begin(); it != countNormal[i].end(); ++it)
		{
			vtxNormal[i].x += it->x;
			vtxNormal[i].y += it->y;
			vtxNormal[i].z += it->z;
		}
		vtxNormal[i].x /= countNormal[i].size();
		vtxNormal[i].y /= countNormal[i].size();
		vtxNormal[i].z /= countNormal[i].size();

		Vector3 vtx = vtxNormal[i];
		vtx.Normalize();
	}

	return vtxNormal;
}
