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
	void Init(const char* filePath, int levels = 64 , int slices = 64);

	/// @brief 
	/// @param tex 
	///	@param levels 球体垂直分割数
	///	@param slices 球体・シリンダー水平分割数
	void Init(std::shared_ptr<Texture>tex, int levels = 64, int slices = 64);
	

	void Update(float dt);
	void Draw(int texSlot);

private:

	/// @brief メッシュ作成
	///	@param levels 球体垂直分割数
	///	@param slices 球体・シリンダー水平分割数
	/// @param stacks シリンダー垂直分割数
	void CreateMesh(UINT levels, UINT slices, UINT stacks);

	void WriteDefShader();

};

