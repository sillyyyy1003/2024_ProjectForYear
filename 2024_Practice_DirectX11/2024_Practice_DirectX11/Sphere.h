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


	void Init(const char* _fileName);
	void Update(float dt);
	void Draw(int texSlot = 0);


private:
	/// @brief Mesh初期化
	///	@param levels 垂直分割数
	///	@param slices 水平分割数
	const void CreateMesh(UINT levels, UINT slices);

	/// @brief マテリアル初期化 default Color:White
	const void CreateMaterial();

	/// @brief Texture初期化
	/// @param fileName  File Root
	const void CreateTexture(const char* fileName);

	/*DefShader
	 * 使用方向光
	 * 使用Default Camera
	 * todo:需要进行修改来对应实际的游戏情况...加油！
	 */
	/// @brief Default Shaderに使われるデータの書き込み
	const void WriteDefShader();



};

