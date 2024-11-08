#pragma once
#include "Primitive.h"

class CylinderOneCap :public Primitive
{
private:

	std::unique_ptr<Mesh> mMesh = nullptr;

public:
	CylinderOneCap();
	~CylinderOneCap() override = default;

	void Init(const char* filePath);
	void Update(float dt);
	void Draw(int texSlot);

private:

	/// @brief 円柱メッシュ作成
	/// @param slices 垂直分割数
	/// @param stacks 水平分割数
	void CreateMesh(UINT slices, UINT stacks);

	void CreateMaterial();
	void CreateTexture(const char* filePath);

	void WriteDefShader();
};

