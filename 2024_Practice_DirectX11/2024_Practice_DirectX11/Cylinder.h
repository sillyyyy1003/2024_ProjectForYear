#pragma once
#include "Primitive.h"

class Cylinder : public Primitive
{
private:

	std::unique_ptr<Mesh> mMesh = nullptr;
	MaterialData mMaterial = {};

public:
	Cylinder();
	~Cylinder() override = default;

	void Init(const char* filePath);
	void Update(float dt);
	void Draw(int texSlot);

private:

	/// @brief �~�����b�V���쐬
	/// @param slices ����������
	/// @param stacks ����������
	void CreateMesh(UINT slices, UINT stacks);

	void CreateMaterial();
	void CreateTexture(const char* filePath);

	void WriteDefShader();
};
