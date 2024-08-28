#include "Geometry.h"
#include "Mesh.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

Geometry::Data Geometry::mData;

Geometry::~Geometry()
{
}

void Geometry::Init()
{
	for (int i = 0; i < 3; i++)
	{
		DirectX::XMStoreFloat4x4(&mData.matrix[i], DirectX::XMMatrixIdentity());
		
	}

	mData.param[0] = Color(1.f, 1.f, 1.f, 1.f);//lightColor
	mData.param[1] = XMFLOAT4(1.f, 1.f, 1.f, 1.f);//lightDir&lightSwitch

	CreateVS();
	CreatePS();
	CreateBoxMesh();
	CreateSphereMesh(20,20);
	CreateCylinder(20, 1);
	CreateCylinderNoCap(20, 20);
	CreateCone(20);
	CreatePlane3D();
	CreateCapsule(20,20,1);
}

void Geometry::DrawBox()
{
	mData.mVS->WriteShader(0, mData.matrix);
	mData.mVS->WriteShader(1, mData.param);
	mData.mVS->SetShader();
	mData.mPS->SetShader();
	mData.mBoxMesh->Draw();
}

void Geometry::DrawSphere()
{
	mData.mVS->WriteShader(0, mData.matrix);
	mData.mVS->WriteShader(1, mData.param);
	mData.mVS->SetShader();
	mData.mPS->SetShader();
	mData.mSphereMesh->Draw();
}

void Geometry::DrawCylinder()
{
	mData.mVS->WriteShader(0, mData.matrix);
	mData.mVS->WriteShader(1, mData.param);
	mData.mVS->SetShader();
	mData.mPS->SetShader();
	mData.mCylinderMesh->Draw();
}

void Geometry::DrawCylinderNoCap()
{
	mData.mVS->WriteShader(0, mData.matrix);
	mData.mVS->WriteShader(1, mData.param);
	mData.mVS->SetShader();
	mData.mPS->SetShader();
	mData.mCylinderNoCapMesh->Draw();
}

void Geometry::DrawCone()
{
	mData.mVS->WriteShader(0, mData.matrix);
	mData.mVS->WriteShader(1, mData.param);
	mData.mVS->SetShader();
	mData.mPS->SetShader();
	mData.mConeMesh->Draw();
}

void Geometry::DrawPlane3D()
{
	mData.mVS->WriteShader(0, mData.matrix);
	mData.mVS->WriteShader(1, mData.param);
	mData.mVS->SetShader();
	mData.mPS->SetShader();
	mData.mPlane3DMesh->Draw();
}

void Geometry::DrawCapsule()
{
	mData.mVS->WriteShader(0, mData.matrix);
	mData.mVS->WriteShader(1, mData.param);
	mData.mVS->SetShader();
	mData.mPS->SetShader();
	mData.mCapsuleMesh->Draw();
}

void Geometry::SetLightDir(DirectX::XMFLOAT3 _lightDir)
{
	DirectX::XMVECTOR vDir = DirectX::XMLoadFloat3(&_lightDir);
	//正規化
	DirectX::XMStoreFloat3(&_lightDir, DirectX::XMVector3Normalize(vDir));
	mData.param[1].x = _lightDir.x;
	mData.param[1].y = _lightDir.y;
	mData.param[1].z = _lightDir.z;
}

void Geometry::UnInit()
{
	mData.mSphereMesh.reset();//球体
	mData.mBoxMesh.reset();//ボックス
	mData.mConeMesh.reset();//円錐
	mData.mCylinderMesh.reset();//円柱
	mData.mCylinderNoCapMesh.reset();//Cylinder Without caps
	mData.mPlane3DMesh.reset();//平面
	mData.mCapsuleMesh.reset();

	mData.mVS.reset();
	mData.mPS.reset();
}



void Geometry::CreateVS()
{
	mData.mVS = std::make_unique<VertexShader>();
	HR(mData.mVS->LoadShaderFile("Assets/Shader/VS_Geometry.cso"));

}

void Geometry::CreatePS()
{
	mData.mPS = std::make_unique<PixelShader>();
	HR(mData.mPS->LoadShaderFile("Assets/Shader/PS_Geometry.cso"));
}

void Geometry::CreateSphereMesh(UINT levels, UINT slices)
{
	std::vector<Vertex::VtxPosColorNormal> vtx;

	// Generate vertices
	for (UINT j = 0; j <= levels; ++j)
	{
		float theta = DirectX::XM_PI * j / levels; // latitude angle (0 to PI)
		float y = cosf(theta); // y position
		float sinTheta = sinf(theta);

		for (UINT i = 0; i <= slices; ++i)
		{
			float phi = DirectX::XM_2PI * i / slices; // longitude angle (0 to 2*PI)
			float x = sinTheta * cosf(phi); // x position
			float z = sinTheta * sinf(phi); // z position

			vtx.push_back({
				DirectX::XMFLOAT3(x * 0.5f, y * 0.5f, z * 0.5f),
				DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
				DirectX::XMFLOAT3(x, y, z)
				});
		}
	}
	std::vector<DWORD> idx;
	// Generate indices
	for (UINT j = 0; j < levels; ++j)
	{
		for (UINT i = 0; i < slices; ++i)
		{
			idx.push_back(j * (slices + 1) + i);
			idx.push_back((j + 1) * (slices + 1) + i);
			idx.push_back(j * (slices + 1) + i + 1);

			idx.push_back(j * (slices + 1) + i + 1);
			idx.push_back((j + 1) * (slices + 1) + i);
			idx.push_back((j + 1) * (slices + 1) + i + 1);
		}
	}

	Mesh::MeshData desc = {};
	desc.pVertex = vtx.data();
	desc.vertexSize = sizeof(Vertex::VtxPosColorNormal);
	desc.vertexCount = static_cast<UINT>(vtx.size());
	desc.pIndex = idx.data();
	desc.indexSize = sizeof(idx[0]);
	desc.indexCount = static_cast<UINT>(idx.size());
	desc.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	mData.mSphereMesh = std::make_unique<Mesh>(desc);


}

void Geometry::CreateBoxMesh()
{
	const float d = 0.5f;
	DirectX::XMFLOAT4 color(1.0f, 1.0f, 1.0f, 1.0f);

	DirectX::XMFLOAT3 normal[] = {
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

	//todo:if use tangent need to fix the vertex
	std::vector<Vertex::VtxPosColorNormal> vtx;
	vtx.resize(24);

	//Set Color&pos
	for (int i = 0; i < 24; i++)
	{
		vtx[i].pos = pos[i];
		vtx[i].color = color;
	}
	//Set Normal
	for (int i = 0; i < 4; ++i)
	{
		// 右面(+X面)
		vtx[i].normal = XMFLOAT3(1.0f, 0.0f, 0.0f);

		// 左面(-X面)
		vtx[i + 4].normal = XMFLOAT3(-1.0f, 0.0f, 0.0f);

		// 顶面(+Y面)
		vtx[i + 8].normal = XMFLOAT3(0.0f, 1.0f, 0.0f);

		// 底面(-Y面)
		vtx[i + 12].normal = XMFLOAT3(0.0f, -1.0f, 0.0f);

		// 背面(+Z面)
		vtx[i + 16].normal = XMFLOAT3(0.0f, 0.0f, 1.0f);

		// 正面(-Z面)
		vtx[i + 20].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

	}

	std::vector<DWORD> idx = {
		0, 1, 2, 2, 3, 0,        // 右面(+X面)
		4, 5, 6, 6, 7, 4,        // 左面(-X面)
		8, 9, 10, 10, 11, 8,    // 顶面(+Y面)
		12, 13, 14, 14, 15, 12,    // 底面(-Y面)
		16, 17, 18, 18, 19, 16, // 背面(+Z面)
		20, 21, 22, 22, 23, 20    // 正面(-Z面)
	};
	

	Mesh::MeshData desc = {};
	desc.pVertex = vtx.data();
	desc.vertexSize = sizeof(Vertex::VtxPosColorNormal);
	desc.vertexCount = static_cast<UINT>(vtx.size());
	desc.pIndex = idx.data();
	desc.indexSize = sizeof(DWORD);
	desc.indexCount = static_cast<UINT>(idx.size());
	desc.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	mData.mBoxMesh = std::make_unique<Mesh>(desc);
}

void Geometry::CreateCylinder(UINT slices, UINT stacks)
{
	//todo:不加入texcoord
	std::vector<Vertex::VtxPosColorNormal> vtx;
	std::vector<DWORD> idx;

	float height = 1.f;
	float radius = 0.5f;

	float h2 = 1.f / 2.0f;
	float per_theta = DirectX::XM_2PI / slices;
	float per_stack = height / stacks;
	Color color = Color(1.f, 1.f, 1.f, 1.f);
	// Generate vertices

	// Cylinder body vertices
	for (UINT j = 0; j <= stacks; ++j)
	{
		float y = h2 - j * per_stack;
		for (UINT i = 0; i <= slices; ++i)
		{
			float theta = i * per_theta;
			float x = radius * cosf(theta);
			float z = radius * sinf(theta);
			vtx.push_back({
				DirectX::XMFLOAT3(x, y, z),
				color,
				DirectX::XMFLOAT3(x, 0.0f, z)
				});
		}
	}

	// Top cap center vertex
	vtx.push_back({
		DirectX::XMFLOAT3(0.0f, h2, 0.0f),
		color,
		DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f)
		});

	// Top cap vertices
	for (UINT i = 0; i <= slices; ++i)
	{
		float theta = i * per_theta;
		float x = radius * cosf(theta);
		float z = radius * sinf(theta);
		vtx.push_back({
			DirectX::XMFLOAT3(x, h2, z),
			color,
			DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f)
			});
	}

	// Bottom cap center vertex
	vtx.push_back({
		DirectX::XMFLOAT3(0.0f, -h2, 0.0f),
		color,
		DirectX::XMFLOAT3(0.0f, -1.0f, 0.0f)
		});

	// Bottom cap vertices
	for (UINT i = 0; i <= slices; ++i)
	{
		float theta = i * per_theta;
		float x = radius * cosf(theta);
		float z = radius * sinf(theta);
		vtx.push_back({
			DirectX::XMFLOAT3(x, -h2, z),
			color,
			DirectX::XMFLOAT3(0.0f, -1.0f, 0.0f)
			});
	}

	// Generate indices
	// Cylinder body indices
	for (UINT j = 0; j < stacks; ++j)
	{
		for (UINT i = 0; i < slices; ++i)
		{
			idx.push_back(j * (slices + 1) + i);
			idx.push_back((j + 1) * (slices + 1) + i);
			idx.push_back(j * (slices + 1) + i + 1);

			idx.push_back(j * (slices + 1) + i + 1);
			idx.push_back((j + 1) * (slices + 1) + i);
			idx.push_back((j + 1) * (slices + 1) + i + 1);
		}
	}

	// Top cap indices
	UINT offset = (slices + 1) * (stacks + 1);
	UINT centerIndex = offset;
	for (UINT i = 1; i <= slices; ++i)
	{
		idx.push_back(centerIndex);
		idx.push_back(centerIndex + i);
		idx.push_back(centerIndex + i % (slices + 1) + 1);
	}

	// Bottom cap indices
	offset += slices + 2;
	centerIndex = offset;
	for (UINT i = 1; i <= slices; ++i)
	{
		idx.push_back(centerIndex);
		idx.push_back(centerIndex + i % (slices + 1) + 1);
		idx.push_back(centerIndex + i);
	}

	Mesh::MeshData desc = {};
	desc.pVertex = vtx.data();
	desc.vertexSize = sizeof(Vertex::VtxPosColorNormal);
	desc.vertexCount = static_cast<UINT>(vtx.size());
	desc.pIndex = idx.data();
	desc.indexSize = sizeof(DWORD);
	desc.indexCount = static_cast<UINT>(idx.size());
	desc.topology = D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP;
	mData.mCylinderMesh = std::make_unique<Mesh>(desc);

}

void Geometry::CreateCylinderNoCap(UINT slices, UINT stacks)
{
	std::vector<Vertex::VtxPosColorNormal> vtx;
	std::vector<DWORD> idx;

	float radius = 0.5f;
	float height = 1.0f;
	Color color = Color(1.f, 1.f, 1.f, 1.f);
	float h2 = height / 2.0f;
	float per_theta = DirectX::XM_2PI / slices;
	float stackHeight = height / stacks;

	// Generate vertices
	// Cylinder body vertices
	for (UINT j = 0; j <= stacks; ++j)
	{
		float y = h2 - j * stackHeight;
		for (UINT i = 0; i <= slices; ++i)
		{
			float theta = i * per_theta;
			float x = radius * cosf(theta);
			float z = radius * sinf(theta);
			vtx.push_back({
				DirectX::XMFLOAT3(x, y, z),
				color,
				DirectX::XMFLOAT3(x, 0.0f, z)
				});
		}
	}

	// Generate indices
	// Cylinder body indices
	for (UINT j = 0; j < stacks; ++j)
	{
		for (UINT i = 0; i < slices; ++i)
		{
			idx.push_back(j * (slices + 1) + i);
			idx.push_back((j + 1) * (slices + 1) + i);
			idx.push_back(j * (slices + 1) + i + 1);

			idx.push_back(j * (slices + 1) + i + 1);
			idx.push_back((j + 1) * (slices + 1) + i);
			idx.push_back((j + 1) * (slices + 1) + i + 1);
		}
	}

	Mesh::MeshData desc = {};
	desc.pVertex = vtx.data();
	desc.vertexSize = sizeof(Vertex::VtxPosColorNormal);
	desc.vertexCount = static_cast<UINT>(vtx.size());
	desc.pIndex = idx.data();
	desc.indexSize = sizeof(idx[0]);
	desc.indexCount = static_cast<UINT>(idx.size());
	desc.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	mData.mCylinderNoCapMesh = std::make_unique<Mesh>(desc);
}

void Geometry::CreateCone(UINT slices)
{
	std::vector<Vertex::VtxPosColorNormal> vtx;
	std::vector<DWORD> idx;

	float radius = 0.5f;
	float height = 1.0f;
	float h2 = height / 2.0f;
	float per_theta = DirectX::XM_2PI / slices;
	Color color = Color(1.f, 1.f, 1.f, 1.f);

	// Generate vertices
	// Cone body vertices
	for (UINT i = 0; i <= slices; ++i)
	{
		float theta = i * per_theta;
		float x = radius * cosf(theta);
		float z = radius * sinf(theta);
		vtx.push_back({
			DirectX::XMFLOAT3(x, -h2, z),
			color,
			DirectX::XMFLOAT3(x, 0.0f, z)
			});
	}

	// Cone apex vertex
	vtx.push_back({
		DirectX::XMFLOAT3(0.0f, h2, 0.0f),
		color,
		DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f)
		});

	// Bottom cap center vertex
	vtx.push_back({
		DirectX::XMFLOAT3(0.0f, -h2, 0.0f),
		color,
		DirectX::XMFLOAT3(0.0f, -1.0f, 0.0f)
		});

	// Generate indices
	// Cone body indices
	UINT apexIndex = static_cast<UINT>(vtx.size()) - 2;
	for (UINT i = 0; i < slices; ++i)
	{
		idx.push_back(apexIndex);
		idx.push_back(i);
		idx.push_back((i + 1) % slices);
	}

	// Bottom cap indices
	UINT centerIndex = static_cast<UINT>(vtx.size()) - 1;
	for (UINT i = 0; i < slices; ++i)
	{
		idx.push_back(centerIndex);
		idx.push_back(i);
		idx.push_back((i + 1) % slices);
	}

	Mesh::MeshData desc = {};
	desc.pVertex = vtx.data();
	desc.vertexSize = sizeof(Vertex::VtxPosColorNormal);
	desc.vertexCount = static_cast<UINT>(vtx.size());
	desc.pIndex = idx.data();
	desc.indexSize = sizeof(DWORD);
	desc.indexCount = static_cast<UINT>(idx.size());
	desc.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;//LINELIST;//TRIANGLELIST;
	mData.mConeMesh = std::make_unique<Mesh>(desc);
}

void Geometry::CreatePlane3D()
{
	const float d = 0.5f;
	DirectX::XMFLOAT4 color(1.0f, 1.0f, 1.0f, 1.0f);

	DirectX::XMFLOAT3 pos[] = {
		
		XMFLOAT3(-d, 0, -d),
		XMFLOAT3(-d, 0, d),
		XMFLOAT3(d, 0, d),
		XMFLOAT3(d, 0, -d),

		XMFLOAT3(d, 0, -d),
		XMFLOAT3(d, 0, d),
		XMFLOAT3(-d, 0, d),
		XMFLOAT3(-d,0, -d),
		
	};

	//todo:if use tangent need to fix the vertex
	std::vector<Vertex::VtxPosColorNormal> vtx;
	vtx.resize(8);

	//Set Color&pos
	for (int i = 0; i < 4; i++)
	{
		vtx[i].pos = pos[i];
		vtx[i].color = color;
		vtx[i].normal = XMFLOAT3(0.0f, 1.0f, 0.0f);

		vtx[i + 4].pos = pos[i + 4];
		vtx[i + 4].color = color;
		vtx[i + 4].normal = XMFLOAT3(0.0f, -1.0f, 0.0f);
	}
	

	std::vector<DWORD> idx = {
		0, 1, 2,  2, 3, 0,//+Y
		4, 5, 6,  6, 7, 4,//-Y
	};


	Mesh::MeshData desc = {};
	desc.pVertex = vtx.data();
	desc.vertexSize = sizeof(Vertex::VtxPosColorNormal);
	desc.vertexCount = static_cast<UINT>(vtx.size());
	desc.pIndex = idx.data();
	desc.indexSize = sizeof(DWORD);
	desc.indexCount = static_cast<UINT>(idx.size());
	desc.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	mData.mPlane3DMesh = std::make_unique<Mesh>(desc);

}

void Geometry::CreateCapsule(UINT levels,UINT slices, UINT stacks)
{
	std::vector<Vertex::VtxPosColorNormal> vtx;
	std::vector<DWORD> idx;

	// Generate vertices

	float radius = 0.5f;
	float height = 1.0f;
	Color color = Color(1.f, 1.f, 1.f, 1.f);
	float h2 = height / 2.0f;
	float per_theta = DirectX::XM_2PI / slices;
	float stackHeight = height / stacks;

	// Cylinder body vertices
	for (UINT j = 0; j <= stacks; ++j)
	{
		float y = h2 - j * stackHeight;
		for (UINT i = 0; i <= slices; ++i)
		{
			float theta = i * per_theta;
			float x = radius * cosf(theta);
			float z = radius * sinf(theta);
			vtx.push_back({
				DirectX::XMFLOAT3(x, y, z),
				color,
				DirectX::XMFLOAT3(x, 0.0f, z)
				});
		}
	}

	// Up hemisphere vertices
	for (UINT j = 0; j <= levels / 2; ++j)
	{
		float theta = DirectX::XM_PI * j / levels; // latitude angle (0 to PI/2 for hemisphere)
		float y = cosf(theta); // y position
		float sinTheta = sinf(theta);

		for (UINT i = 0; i <= slices; ++i)
		{
			float phi = DirectX::XM_2PI * i / slices; // longitude angle (0 to 2*PI)
			float x = sinTheta * cosf(phi); // x position
			float z = sinTheta * sinf(phi); // z position

			vtx.push_back({
				DirectX::XMFLOAT3(x * 0.5f, 0.5f + y * 0.5f, z * 0.5f),
				DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
				DirectX::XMFLOAT3(x, y, z)
				});
		}
	}

	// Lower hemisphere vertices
	for (UINT j = levels / 2; j <= levels; ++j)
	{
		float theta = DirectX::XM_PI * j / levels; // latitude angle (PI/2 to PI)
		float y = cosf(theta); // y position
		float sinTheta = sinf(theta);

		for (UINT i = 0; i <= slices; ++i)
		{
			float phi = DirectX::XM_2PI * i / slices; // longitude angle (0 to 2*PI)
			float x = sinTheta * cosf(phi); // x position
			float z = sinTheta * sinf(phi); // z position

			vtx.push_back({
				DirectX::XMFLOAT3(x * 0.5f, -0.5f + y * 0.5f, z * 0.5f),
				DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
				DirectX::XMFLOAT3(x, y, z)
				});
		}
	}

	// Cylinder body indices
	for (UINT j = 0; j < stacks; ++j)
	{
		for (UINT i = 0; i < slices; ++i)
		{
			idx.push_back(j * (slices + 1) + i);
			idx.push_back((j + 1) * (slices + 1) + i);
			idx.push_back(j * (slices + 1) + i + 1);

			idx.push_back(j * (slices + 1) + i + 1);
			idx.push_back((j + 1) * (slices + 1) + i);
			idx.push_back((j + 1) * (slices + 1) + i + 1);
		}
	}

	// Up hemisphere indices
	UINT offset = (slices + 1) * (stacks + 1);
	for (UINT j = 0; j < levels / 2; ++j)
	{
		for (UINT i = 0; i < slices; ++i)
		{
			idx.push_back(offset + j * (slices + 1) + i);
			idx.push_back(offset + (j + 1) * (slices + 1) + i);
			idx.push_back(offset + j * (slices + 1) + i + 1);

			idx.push_back(offset + j * (slices + 1) + i + 1);
			idx.push_back(offset + (j + 1) * (slices + 1) + i);
			idx.push_back(offset + (j + 1) * (slices + 1) + i + 1);
		}
	}

	// Lower hemisphere indices
	offset += (slices + 1) * (levels / 2 + 1);
	for (UINT j = 0; j < levels / 2; ++j)
	{
		for (UINT i = 0; i < slices; ++i)
		{
			idx.push_back(offset + j * (slices + 1) + i);
			idx.push_back(offset + (j + 1) * (slices + 1) + i);
			idx.push_back(offset + j * (slices + 1) + i + 1);

			idx.push_back(offset + j * (slices + 1) + i + 1);
			idx.push_back(offset + (j + 1) * (slices + 1) + i);
			idx.push_back(offset + (j + 1) * (slices + 1) + i + 1);
		}
	}


	
	// Create MeshData
	Mesh::MeshData desc = {};
	desc.pVertex = vtx.data();
	desc.vertexSize = sizeof(Vertex::VtxPosColorNormal);
	desc.vertexCount = static_cast<UINT>(vtx.size());
	desc.pIndex = idx.data();
	desc.indexSize = sizeof(DWORD);
	desc.indexCount = static_cast<UINT>(idx.size());
	desc.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	mData.mCapsuleMesh = std::make_unique<Mesh>(desc);
}


