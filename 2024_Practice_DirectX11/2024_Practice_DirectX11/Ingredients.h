#pragma once
#include "Primitive.h"

/// Potionを構成する素材
class Ingredients
{

	std::unique_ptr<Primitive> mMesh = nullptr;
	float mPrice = 0.0f;



public:
	Ingredients();
	~Ingredients() = default;

	/// @brief メッシュの種類を確定する
	///	@param kind MeshKind(Box/Plane3D/Cylinder...)
	void InitPrimitive(PrimitiveKind kind);

	/// @brief テクスチャを読み込み
	/// @param filePath ファイルパスs
	void InitTex(const char* filePath);


	void Update(float dt);
	void Draw();

	/// @brief 拖拽物体移动，只考虑两个轴
	void Drag();


};

