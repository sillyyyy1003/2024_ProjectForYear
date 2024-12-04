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
	///	@param _UVSplit UVAnimation
	void Init(const char* filePath, int slices = 64, int stacks = 64, DirectX::XMINT2 _UVSplit = { 1,1 })override;

	/// @brief Create square with existing;
	///	@param tex
	/// @param slices ����������
	/// @param stacks ����������
	///	@param _UVSplit UVAnimation
	void Init(const std::shared_ptr<Texture>& tex, int slices = 64, int stacks = 64,DirectX::XMINT2 _UVSplit = { 1,1 })override;

	void Draw(int texSlot = 0) override;
	void Update(float dt) override;

private:

	/// @brief �~�����b�V���쐬
	/// @param slices ����������
	/// @param stacks ����������
	void CreateMesh(UINT slices, UINT stacks)override;

	void WriteDefShader() override;
};

