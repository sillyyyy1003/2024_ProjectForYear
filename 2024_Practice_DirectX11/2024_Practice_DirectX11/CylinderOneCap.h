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

	/// @brief ‰~’ŒƒƒbƒVƒ…ì¬
	/// @param slices ‚’¼•ªŠ„”
	/// @param stacks …•½•ªŠ„”
	void CreateMesh(UINT slices, UINT stacks);

	void CreateMaterial();
	void CreateTexture(const char* filePath);

	void WriteDefShader();
};

