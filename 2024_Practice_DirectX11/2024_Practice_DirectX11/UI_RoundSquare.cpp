#include "UI_RoundSquare.h"

using namespace DirectX::SimpleMath;
using namespace DirectX;

UI_RoundSquare::UI_RoundSquare():UI_Primitive(UIPrimitiveConfig::UI_PrimitiveKind::ROUND_SQUARE)
{
}

UI_RoundSquare::~UI_RoundSquare()
{
}

void UI_RoundSquare::Init(const std::shared_ptr<Texture>& pTex, DirectX::XMFLOAT2 _squareSize, float radius, int slices,
	DirectX::XMINT2 _UVSplit)
{


	//UV Animation を使うか
	if (_UVSplit.x == 1 && _UVSplit.y == 1)
		isUseUVAnimation = false;
	else
		isUseUVAnimation = true;
	
	mUvAnimation = std::make_unique<UVAnimation>();
	mUvAnimation->Init(_UVSplit);
	//パラメータ
	mRoundRadius = radius;
	mSquareSize = _squareSize;
	//Init mesh/material/Texture
	CreateMesh(slices);
	CreateMaterial();
	CreateTexture(pTex);

}

void UI_RoundSquare::Update()
{
	UI_Primitive::Update();
}

void UI_RoundSquare::Draw(int texSlot)
{
	SetDefShader();
	mVS->SetShader();
	mPS->SetShader();
	if (mMaterial.tex)
		mDefPS->SetTexture(0, mMaterial.tex.get());
	mMesh->Draw();
}

void UI_RoundSquare::CreateMesh(int slices)
{
    
    //float radius = mRoundRadius;  // 圆角半径
    //float length = mSquareSize.x; // 矩形的宽度
    //float height = mSquareSize.y; // 矩形的高度

    //std::vector<Vertex::VtxPosNormalTex> vtx;     // 顶点数组
    //std::vector<DWORD> indexData;                 // 索引数组

    //// 定义四个圆角的圆心
    //Vector2 centers[4] = {
    //    {-length / 2 + radius, height / 2 - radius},  // 左上
    //    {length / 2 - radius, height / 2 - radius},   // 右上
    //    {length / 2 - radius, -height / 2 + radius},  // 右下
    //    {-length / 2 + radius, -height / 2 + radius}  // 左下
    //};

    //// 定义四个圆角的起始角度
    //float startAngles[4] = {
    //    DirectX::XM_PIDIV2,  // 左上从 90°
    //    0.0f,                // 右上从 0°
    //    -DirectX::XM_PIDIV2, // 右下从 -90°
    //    DirectX::XM_PI       // 左下从 180°
    //};

    //// 遍历四个圆角，生成顶点和索引
    //int vertexBaseIndex = 0;
    //for (int corner = 0; corner < 4; ++corner)
    //{
    //    Vector2 center = centers[corner];   // 当前圆心位置
    //    float startAngle = startAngles[corner];

    //    // 添加圆心顶点到 vtx
    //    vtx.push_back({ Vector3(center.x, center.y, 0.0f), Vector3(0.0f, 0.0f, -1.0f), Vector2(center.x, center.y) });

    //    // 添加圆弧上的顶点
    //    for (int i = 0; i <= slices; ++i)
    //    {
    //        float theta = startAngle + i * (DirectX::XM_PIDIV2 / slices);
    //        float x = center.x + radius * cosf(theta);
    //        float y = center.y + radius * sinf(theta);

    //        // 添加弧线上的顶点
    //        vtx.push_back({ Vector3(x, y, 0.0f), Vector3(0.0f, 0.0f, -1.0f), Vector2((x + length / 2) / length, (y + height / 2) / height) });
    //    }

    //    // 生成当前圆角的索引数据（三角形，圆心与弧线的每个片段）
    //    for (int i = 0; i < slices; i++)
    //    {
    //        indexData.push_back(vertexBaseIndex);        // 圆心索引
    //        indexData.push_back(vertexBaseIndex + i + 1); // 当前弧线索引
    //        indexData.push_back(vertexBaseIndex + i + 2); // 下一弧线索引
    //    }

    //    DWORD finalIdx = indexData[indexData.size() - 1];
    //    indexData.push_back(vertexBaseIndex);
    //    indexData.push_back(finalIdx);
    //    indexData.push_back()
    //    
    //    // 更新基础顶点索引，跳过当前圆心和弧线段的所有顶点
    //    vertexBaseIndex += slices + 2; // 当前圆角：1 圆心 + (slices + 1) 弧线顶点
    //}

    //// 创建网格
    //Mesh::MeshData desc = {};
    //desc.pVertex = vtx.data();
    //desc.vertexSize = sizeof(Vertex::VtxPosNormalTex);
    //desc.vertexCount = static_cast<UINT>(vtx.size());
    //desc.pIndex = indexData.data();
    //desc.indexSize = sizeof(DWORD);
    //desc.indexCount = static_cast<UINT>(indexData.size());
    //desc.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

    //mMesh = std::make_unique<Mesh>(desc);
    //SetVertices(vtx);
}

void UI_RoundSquare::WriteDefShader()
{
	XMMATRIX WVP[3] = {};
	WVP[0] = mTransform.GetMatrix();
	WVP[1] = XMMatrixIdentity();
	WVP[2] = XMMatrixOrthographicLH(WIN_WIDTH,WIN_HEIGHT, 0.0f, 3.0f);
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
