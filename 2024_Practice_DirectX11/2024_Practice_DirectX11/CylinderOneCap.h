#pragma once
#include "Primitive.h"

class CylinderOneCap :public Primitive
{
private:

	std::unique_ptr<Mesh> mMesh = nullptr;

public:
	CylinderOneCap();
	~CylinderOneCap() override = default;

	/// @brief 
	/// @param filePath 
	/// @param slices ����������
	/// @param stacks ����������
	void Init(const char* filePath, int slices = 64 , int stacks = 64);

	/// @brief Create square with existing;
	///	@param tex
	/// @param slices ����������
	/// @param stacks ����������
	void Init(std::shared_ptr<Texture> tex, int slices = 64, int stacks = 64);


	void Update(float dt);
	void Draw(int texSlot);

private:

	/// @brief �~�����b�V���쐬
	/// @param slices ����������
	/// @param stacks ����������
	void CreateMesh(UINT slices, UINT stacks);

	void WriteDefShader();
};

