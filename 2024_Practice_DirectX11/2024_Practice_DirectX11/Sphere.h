#pragma once
#include "D3DUtil.h"
#include "Primitive.h"
#include "Shader.h"
#include "Transform.h"

class Sphere :public Primitive
{
private:

	std::unique_ptr<Mesh> mMesh; //描画

public:

	Sphere();
	~Sphere() override = default;


	/// @brief 
	/// @param filePath 
	///	@param levels 球体垂直分割数
	///	@param slices 球体・シリンダー水平分割数
	void Init(const char* filePath, int levels = 64, int slices = 64)override;

	/// @brief 
	/// @param tex 
	///	@param levels 球体垂直分割数
	///	@param slices 球体・シリンダー水平分割数
	void Init(const std::shared_ptr<Texture>& tex, int levels = 64, int slices = 64) override;


	void Update(float dt)override;
	void Draw(int texSlot = 0)override;

	void WriteDefShader()override;
private:
	/// @brief Mesh初期化
	///	@param levels 垂直分割数
	///	@param slices 水平分割数
	void CreateMesh(UINT levels, UINT slices)override;

	
};

