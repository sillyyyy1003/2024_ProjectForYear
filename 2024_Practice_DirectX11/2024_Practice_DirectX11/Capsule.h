#pragma once
#include "Primitive.h"
class Capsule :public Primitive
{
private:
	std::unique_ptr<Mesh> mMesh = nullptr;
	float mRadius = 1.0f;
	float mHeight = 1.0f;

public:
	Capsule();
	~Capsule() override = default;

	/// @brief 
	/// @param filePath 
	///	@param levels 球体垂直分割数
	///	@param slices 球体・シリンダー水平分割数
	///	@param _UVSplit UVAnimation
	void Init(const char* filePath, int levels = 16 , int slices = 16, DirectX::XMINT2 _UVSplit={1,1}) override;
	
	/// @brief 
	/// @param tex 
	///	@param levels 球体垂直分割数
	///	@param slices 球体・シリンダー水平分割数
	///	@param _UVSplit UVAnimation
	void Init(const std::shared_ptr<Texture>& tex, int levels = 16, int slices = 16, DirectX::XMINT2 _UVSplit={1,1}) override;

	void Update(float dt) override;
	void Draw(int texSlot = 0) override;

private:

	/// @brief メッシュ作成
	///	@param levels 球体垂直分割数
	///	@param slices 球体・シリンダー水平分割数
	/// @param stacks シリンダー垂直分割数
	void CreateMesh(UINT levels, UINT slices, UINT stacks);

	void WriteDefShader() override;

};

