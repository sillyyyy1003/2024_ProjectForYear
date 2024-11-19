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
	void Init(const char* filePath, int levels = 64, int slices = 64);

	/// @brief 
	/// @param tex 
	///	@param levels 球体垂直分割数
	///	@param slices 球体・シリンダー水平分割数
	void Init(std::shared_ptr<Texture>tex, int levels = 64, int slices = 64);


	void Update(float dt);
	void Draw(int texSlot = 0);


private:
	/// @brief Mesh初期化
	///	@param levels 垂直分割数
	///	@param slices 水平分割数
	void CreateMesh(UINT levels, UINT slices);

	/*DefShader
	 * 使用方向光
	 * 使用Default Camera
	 * todo:需要进行修改来对应实际的游戏情况...加油！
	 */
	/// @brief Default Shaderに使われるデータの書き込み
	const void WriteDefShader();
};

