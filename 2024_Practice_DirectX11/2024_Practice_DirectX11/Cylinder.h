#pragma once
#include "Primitive.h"

class Cylinder : public Primitive
{
private:

	std::unique_ptr<Mesh> mMesh = nullptr;
	
public:
	Cylinder();
	~Cylinder() override = default;

	/// @brief 
	/// @param filePath 
	/// @param slices ����������
	/// @param stacks ����������
	void Init(const char* filePath, int slices = 64, int stacks = 64);

	/// @brief Create square with existing;
	///	@param tex
	/// @param slices ����������
	/// @param stacks ����������
	void Init(const std::shared_ptr<Texture>& tex, int slices = 64, int stacks = 64);


	void Update(float dt);
	void Draw(int texSlot = 0);

private:

	/// @brief �~�����b�V���쐬
	/// @param slices ����������
	/// @param stacks ����������
	void CreateMesh(UINT slices, UINT stacks);

	void WriteDefShader();
};

