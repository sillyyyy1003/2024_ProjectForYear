#include "Geometry.h"
#include "Mesh.h"

using namespace DirectX;
using namespace  DirectX::SimpleMath;

Geometry::Data Geometry::mData;

Geometry::~Geometry()
{
}

void Geometry::Init()
{
	for (int i = 0; i < 3; i++)
	{
		XMStoreFloat4x4(&mData.matrix[i], XMMatrixIdentity());
		
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
	XMVECTOR vDir = XMLoadFloat3(&_lightDir);
	//正規化
	XMStoreFloat3(&_lightDir, XMVector3Normalize(vDir));
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
	Color color = { 1.0f,1.0f,1.0f,1.0f };
	float phi = 0.0f, theta = 0.0f;
	float per_phi = XM_PI / levels;
	float per_theta = XM_2PI / slices;
	float x, y, z;
	float radius = 0.5f;

	// Generate vertices

	//上の頂点
	vtx.push_back({
		Vector3(0.0f, radius, 0.0f),
		color,
		Vector3(0.0f, 1.0f, 0.0f),
		});
	//他
	for (UINT i = 1; i < levels; ++i)
	{
		phi = per_phi * i;

		// slices+1->スタートとエンドは同じけど、uvは逆
		for (UINT j = 0; j <= slices; ++j)
		{
			theta = per_theta * j;
			x = radius * sinf(phi) * cosf(theta);
			y = radius * cosf(phi);
			z = radius * sinf(phi) * sinf(theta);
			Vector3 pos = { x,y,z };
			Vector3 normal;
			XMStoreFloat3(&normal, XMVector3Normalize(XMLoadFloat3(&pos)));

			vtx.push_back({
				pos,
				color,
				normal,
				});
		}
	}
	//下の頂点
	vtx.push_back({
	 Vector3(0.0f, -radius, 0.0f),
		color,
	 Vector3(0.0f, -1.0f, 0.0f),

		});



	std::vector<DWORD> idx;

	if (levels > 1)
	{
		for (UINT i = 1; i <= slices; ++i)
		{
			idx.push_back(0);
			idx.push_back(i % (slices + 1) + 1);
			idx.push_back(i);
		}
	}

	for (UINT i = 1; i < levels - 1; ++i)
	{
		for (UINT j = 1; j <= slices; ++j)
		{
			idx.push_back((i - 1) * (slices + 1) + j);
			idx.push_back((i - 1) * (slices + 1) + j % (slices + 1) + 1);
			idx.push_back(i * (slices + 1) + j % (slices + 1) + 1);

			idx.push_back(i * (slices + 1) + j % (slices + 1) + 1);
			idx.push_back(i * (slices + 1) + j);
			idx.push_back((i - 1) * (slices + 1) + j);
		}
	}

	if (levels > 1)
	{
		for (UINT i = 1; i <= slices; ++i)
		{
			idx.push_back((levels - 2) * (slices + 1) + i);
			idx.push_back((levels - 2) * (slices + 1) + i % (slices + 1) + 1);
			idx.push_back((levels - 1) * (slices + 1) + 1);
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
	XMFLOAT4 color(1.0f, 1.0f, 1.0f, 1.0f);

	Vector3 normal[] = {
		Vector3(1.0f, 0.0f, 0.0f),
		Vector3(-1.0f, 0.0f, 0.0f),
		Vector3(0.0f, 1.0f, 0.0f),
		Vector3(0.0f,-1.0f, 0.0f),
		Vector3(0.0f, 0.0f, 1.0f),
		Vector3(0.0f, 0.0f,-1.0f),
	};

	Vector3 pos[] = {
		//+X面
		Vector3(d, -d, -d),
		Vector3(d, d, -d),
		Vector3(d, d, d),
		Vector3(d, -d, d),
		// -X面
		Vector3(-d, -d, d),
		Vector3(-d, d, d),
		Vector3(-d, d, -d),
		Vector3(-d, -d, -d),
		// +Y面
		Vector3(-d, d, -d),
		Vector3(-d, d, d),
		Vector3(d, d, d),
		Vector3(d, d, -d),
		// -Y面
		Vector3(d, -d, -d),
		Vector3(d, -d, d),
		Vector3(-d, -d, d),
		Vector3(-d, -d, -d),
		// +Z面
		Vector3(d, -d, d),
		Vector3(d, d, d),
		Vector3(-d, d, d),
		Vector3(-d, -d, d),
		// -Z面
		Vector3(-d, -d, -d),
		Vector3(-d, d, -d),
		Vector3(d, d, -d),
		Vector3(d, -d, -d),
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
		vtx[i].normal = Vector3(1.0f, 0.0f, 0.0f);

		// 左面(-X面)
		vtx[i + 4].normal = Vector3(-1.0f, 0.0f, 0.0f);

		// 顶面(+Y面)
		vtx[i + 8].normal = Vector3(0.0f, 1.0f, 0.0f);

		// 底面(-Y面)
		vtx[i + 12].normal = Vector3(0.0f, -1.0f, 0.0f);

		// 背面(+Z面)
		vtx[i + 16].normal = Vector3(0.0f, 0.0f, 1.0f);

		// 正面(-Z面)
		vtx[i + 20].normal = Vector3(0.0f, 0.0f, -1.0f);

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

	std::vector<Vertex::VtxPosColorNormal> vtx;
	std::vector<DWORD> idx;
	Color color = { 1.f,1.f,1.0f,1.0f };
	float height = 1.f;
	float radius = 0.5f;

	float h2 = height / 2;
	float theta = 0.0f;
	float per_theta = XM_2PI / slices;
	float stackHeight = height / stacks;

	// Generate vertices
	//body
	for (UINT i = 0; i <= stacks; i++)
	{
		float y = -h2 + i * stackHeight;
		for (UINT j = 0; j <= slices; j++)
		{
			theta = j * per_theta;
			float x = radius * cosf(theta);
			float z = radius * sinf(theta);

			vtx.push_back({
				Vector3(x, y, z),
				color,
				Vector3(cosf(theta), 0.0f, sinf(theta)),

				});
		}
	}
	//top center
	vtx.push_back({
	Vector3(0.0f, h2, 0.0f),
		color,
	Vector3(0.0f, 1.0f, 0.0f),
		});

	//top cap
	for (UINT i = 0; i <= slices; i++)
	{
		theta = i * per_theta;
		float u = cosf(theta) * radius / height + 0.5f;
		float v = sinf(theta) * radius / height + 0.5f;

		float x = radius * cosf(theta);
		float z = radius * sinf(theta);
		vtx.push_back({
			Vector3(x, h2, z),
			color,
			Vector3(0.0f, 1.0f, 0.0f),
			});
	}

	//bot center
	vtx.push_back({
		Vector3(0.0f, -h2, 0.0f),
		color,
		Vector3(0.0f, -1.0f, 0.0f),

		});

	//bot cap
	for (UINT i = 0; i <= slices; i++)
	{
		theta = i * per_theta;
		float u = cosf(theta) * radius / height + 0.5f;
		float v = sinf(theta) * radius / height + 0.5f;
		float x = radius * cosf(theta);
		float z = radius * sinf(theta);
		vtx.push_back({
			Vector3(x, -h2, z),
			color,
			Vector3(0.0f, -1.0f, 0.0f),
			});
	}

	for (UINT i = 0; i < stacks; i++)
	{
		for (UINT j = 0; j < slices; j++)
		{
			idx.push_back(i * (slices + 1) + j);
			idx.push_back((i + 1) * (slices + 1) + j);
			idx.push_back((i + 1) * (slices + 1) + j + 1);

			idx.push_back(i * (slices + 1) + j);
			idx.push_back((i + 1) * (slices + 1) + j + 1);
			idx.push_back(i * (slices + 1) + j + 1);
		}
	}

	// Top cap indices
	UINT offset = (slices + 1) * (stacks + 1);
	for (UINT i = 1; i <= slices; i++)
	{
		idx.push_back(offset);
		idx.push_back(offset + i % (slices + 1) + 1);
		idx.push_back(offset + i);
	}

	// Bottom cap indices
	offset += slices + 2;
	for (UINT i = 1; i <= slices; i++)
	{
		idx.push_back(offset);
		idx.push_back(offset + i);
		idx.push_back(offset + i % (slices + 1) + 1);
	}

	Mesh::MeshData desc = {};
	desc.pVertex = vtx.data();
	desc.vertexSize = sizeof(Vertex::VtxPosColorNormal);
	desc.vertexCount = static_cast<UINT>(vtx.size());
	desc.pIndex = idx.data();
	desc.indexSize = sizeof(DWORD);
	desc.indexCount = static_cast<UINT>(idx.size());
	desc.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
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
	float per_theta = XM_2PI / slices;
	float stackHeight = height / stacks;
	float theta = 0.0f;

	// Generate vertices
	// Cylinder body vertices
	for (UINT i = 0; i <= stacks; i++)
	{
		float y = -h2 + i * stackHeight;
		for (UINT j = 0; j <= slices; j++)
		{
			theta = j * per_theta;
			float x = radius * cosf(theta);
			float z = radius * sinf(theta);

			float u = theta / XM_2PI;
			float v = 1.0f - (float)i / stacks;

			vtx.push_back({
				Vector3(x, y, z),
				color,
				Vector3(cosf(theta), 0.0f, sinf(theta)),

				});
		}
	}
	// Generate indices
	// Cylinder body indices
	for (UINT i = 0; i < stacks; i++)
	{
		for (UINT j = 0; j < slices; j++)
		{
			idx.push_back(i * (slices + 1) + j);
			idx.push_back((i + 1) * (slices + 1) + j);
			idx.push_back((i + 1) * (slices + 1) + j + 1);

			idx.push_back(i * (slices + 1) + j);
			idx.push_back((i + 1) * (slices + 1) + j + 1);
			idx.push_back(i * (slices + 1) + j + 1);
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
	float per_theta = XM_2PI / slices;
	Color color = Color(1.f, 1.f, 1.f, 1.f);

	// Generate vertices
	// Cone body vertices
	for (UINT i = 0; i <= slices; ++i)
	{
		float theta = i * per_theta;
		float x = radius * cosf(theta);
		float z = radius * sinf(theta);
		vtx.push_back({
			Vector3(x, -h2, z),
			color,
			Vector3(x, 0.0f, z)
			});
	}

	// Cone apex vertex
	vtx.push_back({
		Vector3(0.0f, h2, 0.0f),
		color,
		Vector3(0.0f, 1.0f, 0.0f)
		});

	// Bottom cap center vertex
	vtx.push_back({
		Vector3(0.0f, -h2, 0.0f),
		color,
		Vector3(0.0f, -1.0f, 0.0f)
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
	XMFLOAT4 color(1.0f, 1.0f, 1.0f, 1.0f);

	Vector3 pos[] = {
		
		Vector3(-d, 0, -d),
		Vector3(-d, 0, d),
		Vector3(d, 0, d),
		Vector3(d, 0, -d),

		Vector3(d, 0, -d),
		Vector3(d, 0, d),
		Vector3(-d, 0, d),
		Vector3(-d,0, -d),
		
	};

	//todo:if use tangent need to fix the vertex
	std::vector<Vertex::VtxPosColorNormal> vtx;
	vtx.resize(8);

	//Set Color&pos
	for (int i = 0; i < 4; i++)
	{
		vtx[i].pos = pos[i];
		vtx[i].color = color;
		vtx[i].normal = Vector3(0.0f, 1.0f, 0.0f);

		vtx[i + 4].pos = pos[i + 4];
		vtx[i + 4].color = color;
		vtx[i + 4].normal = Vector3(0.0f, -1.0f, 0.0f);
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
	float phi = 0.0f, theta = 0.0f;
	float radius = 0.5f;
	float height = 1.0f;
	float h2 = height / 2.0f;
	float per_phi = XM_PI / levels;
	float per_theta = XM_2PI / slices;
	float stackHeight = height / stacks;
	Color color = { 1.0f,1.0f,1.0f,1.0f };

	// Cylinder body vertices
	for (UINT i = 0; i <= stacks; i++)
	{
		float y = -h2 + i * stackHeight;
		for (UINT j = 0; j <= slices; j++)
		{
			theta = j * per_theta;
			float x = radius * cosf(theta);
			float z = radius * sinf(theta);

			vtx.push_back({
				Vector3(x, y, z),
				color,
				Vector3(cosf(theta), 0.0f, sinf(theta)),
			
				});
		}
	}

	//Top Semi_Sphere
	vtx.push_back({
		Vector3(0.0f, radius + h2, 0.0f),
			color,
		Vector3(0.0f, 1.0f, 0.0f),


		});

	for (UINT i = 1; i < levels / 2; i++)
	{
		phi = per_phi * i;

		// slices+1->スタートとエンドは同じけど、uvは逆
		for (UINT j = 0; j <= slices; j++)
		{
			theta = per_theta * j;
			float x = radius * sinf(phi) * cosf(theta);
			float y = radius * cosf(phi) + h2;
			float z = radius * sinf(phi) * sinf(theta);
			Vector3 pos = { x,y,z };
			Vector3 centerToVertex = { x, y - h2, z }; // Subtract h2 to move the vertex relative to the sphere's center
			Vector3 normal;
			XMStoreFloat3(&normal, XMVector3Normalize(XMLoadFloat3(&centerToVertex)));

			vtx.push_back({
				pos,
				color,
				normal,
				});
		}
	}

	//Bot Hemisphere
	for (UINT i = levels / 2; i <= levels; i++)
	{
		phi = per_phi * i;

		// slices+1->スタートとエンドは同じけど、uvは逆
		for (UINT j = 0; j <= slices; j++)
		{
			theta = per_theta * j;
			float x = radius * sinf(phi) * cosf(theta);
			float y = -h2 + radius * cosf(phi);
			float z = radius * sinf(phi) * sinf(theta);
			Vector3 pos = { x,y,z };
			Vector3 centerToVertex = { x, y + h2, z }; // Subtract h2 to move the vertex relative to the sphere's center
			Vector3 normal;
			XMStoreFloat3(&normal, XMVector3Normalize(XMLoadFloat3(&centerToVertex)));

			vtx.push_back({
				pos,
				color,
				normal,
				});
		}
	}

	//Bot point
	vtx.push_back({
		Vector3(0.0f, -h2 - radius, 0.0f),
			color,
		Vector3(0.0f, 1.0f, 0.0f),

		});

	//Cylinder
	for (UINT i = 0; i < stacks; i++)
	{
		for (UINT j = 0; j < slices; j++)
		{
			idx.push_back(i * (slices + 1) + j);
			idx.push_back((i + 1) * (slices + 1) + j);
			idx.push_back((i + 1) * (slices + 1) + j + 1);

			idx.push_back(i * (slices + 1) + j);
			idx.push_back((i + 1) * (slices + 1) + j + 1);
			idx.push_back(i * (slices + 1) + j + 1);
		}
	}

	// Offset to move to the top hemisphere vertices
	UINT offset = (slices + 1) * (stacks + 1);

	// Top hemisphere indices
	for (UINT i = 0; i < slices; i++)
	{
		// Top point of the hemisphere
		idx.push_back(offset + 0);
		idx.push_back(offset + i % (slices + 1) + 1);
		idx.push_back(offset + i);
	}

	for (UINT i = 1; i < levels / 2; i++)
	{
		for (UINT j = 1; j <= slices; j++)
		{
			idx.push_back(offset + (i - 1) * (slices + 1) + j);
			idx.push_back(offset + (i - 1) * (slices + 1) + j % (slices + 1) + 1);
			idx.push_back(offset + i * (slices + 1) + j % (slices + 1) + 1);

			idx.push_back(offset + i * (slices + 1) + j % (slices + 1) + 1);
			idx.push_back(offset + i * (slices + 1) + j);
			idx.push_back(offset + (i - 1) * (slices + 1) + j);
		}
	}

	// Offset to move to the bottom hemisphere vertices
	offset += (slices + 1) * (levels / 2);

	// Bottom hemisphere indices
	for (UINT i = 0; i <= levels / 2; i++)
	{
		for (UINT j = 0; j <= slices; j++)
		{
			idx.push_back(offset + (i - 1) * (slices + 1) + j);
			idx.push_back(offset + (i - 1) * (slices + 1) + j % (slices + 1) + 1);
			idx.push_back(offset + i * (slices + 1) + j % (slices + 1) + 1);

			idx.push_back(offset + i * (slices + 1) + j % (slices + 1) + 1);
			idx.push_back(offset + i * (slices + 1) + j);
			idx.push_back(offset + (i - 1) * (slices + 1) + j);
		}
	}

	for (UINT i = 0; i < slices; i++)
	{
		// Bottom point of the hemisphere
		idx.push_back(offset + (levels - 2) * (slices + 1) + i);
		idx.push_back(offset + (levels - 2) * (slices + 1) + i % (slices + 1) + 1);
		idx.push_back(offset + (levels - 1) * (slices + 1) + 1);
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


