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
	/// @param slices 垂直分割数
	/// @param stacks 水平分割数
	///	@param _UVSplit UVAnimation
	void Init(const char* filePath, int slices = 64, int stacks = 64, DirectX::XMINT2 _UVSplit = { 1,1 })override;

	/// @brief Create square with existing;
	///	@param tex
	/// @param slices 垂直分割数
	/// @param stacks 水平分割数
	///	@param _UVSplit UVAnimation
	void Init(const std::shared_ptr<Texture>& tex, int slices = 64, int stacks = 64, DirectX::XMINT2 _UVSplit = { 1,1 })override;

	void Update(float dt) override;
	void Draw(int texSlot=0) override;

private:

	/// @brief 円柱メッシュ作成
	/// @param slices 垂直分割数
	/// @param stacks 水平分割数
	void CreateMesh(UINT slices, UINT stacks) override;

	void WriteDefShader() override;
};

