#pragma once
#include "D3DUtil.h"
#include "Shader.h"


class Mesh;

///todo: Change as effect base!!!!!
class Geometry
{
private:


	struct Data
	{
		std::unique_ptr<VertexShader> mVS;
		std::unique_ptr<PixelShader> mPS;

		DirectX::XMFLOAT4X4 matrix[3]={};
		DirectX::XMFLOAT4 param[2]={};//param[0] lightColor param[1]->lightDir & lightswitch

		std::unique_ptr<Mesh> mSphereMesh;//球体
		std::unique_ptr<Mesh> mBoxMesh;//ボックス
		std::unique_ptr<Mesh> mConeMesh;//円錐
		std::unique_ptr<Mesh> mCylinderMesh;//円柱
		std::unique_ptr<Mesh> mCylinderNoCapMesh;//Cylinder Without caps
		std::unique_ptr<Mesh> mPlane3DMesh;//平面
		std::unique_ptr<Mesh> mCapsuleMesh;//Capsule
 	};

	static Data mData;

public:
	~Geometry();
	static void Init();
	static void DrawBox();
	static void DrawSphere();
	static void DrawCylinder();
	static void DrawCylinderNoCap();
	static void DrawCone();
	static void DrawPlane3D();
	static void DrawCapsule();


	static void SetWorldMatrix(const DirectX::XMFLOAT4X4& _world) { mData.matrix[0] = _world; }
	static void SetViewMatrix(const DirectX::XMFLOAT4X4& _view) { mData.matrix[1] = _view; }
	static void SetProjMatrix(const DirectX::XMFLOAT4X4& _proj) { mData.matrix[2] = _proj; }

	static void SetColor(DirectX::XMFLOAT4 _color) { mData.param[0] = _color; };
	static void SetLightDir(DirectX::XMFLOAT3 _lightDir);

	static void UnInit();
	
private:
	/// @brief VertexShader読み込む
	static void CreateVS();

	/// @brief PixelShader読み込む
	static void CreatePS();

	/// @brief 球体メッシュ作成
	/// @param levels 
	/// @param slices 
	static void CreateSphereMesh(UINT levels, UINT slices);

	/// @brief ボックスメッシュ作成
	static void CreateBoxMesh();

	/// @brief 円柱メッシュ作成
	/// @param slices 垂直分割数
	/// @param stacks 水平分割数
	static void CreateCylinder(UINT slices, UINT stacks);

	/// @brief 円柱メッシュ（円なし）作成
	/// @param slices 垂直分割数
	/// @param stacks 水平分割数
	static void CreateCylinderNoCap(UINT slices, UINT stacks);

	/// @brief 円錐作成
	/// @param slices 水平分割数
	static void CreateCone(UINT slices);

	/// @brief 平面作成（裏表二面）
	static void CreatePlane3D();

	/// @brief カプセル作成
	///	@param levels 球体垂直分割数
	///	@param slices 球体・シリンダー水平分割数
	/// @param stacks シリンダー垂直分割数
	static void CreateCapsule(UINT levels, UINT slices, UINT stacks);
};



