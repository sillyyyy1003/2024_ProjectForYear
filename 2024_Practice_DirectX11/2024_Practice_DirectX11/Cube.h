#pragma once
#include "Shader.h"
#include "Mesh.h"
#include "Primitive.h"
#include "Texture.h"
#include "Transform.h"

class CameraDCC;

/// @brief Boxの描画
class Cube : public Primitive
{
private:
	//描画関連
	struct MeshData
	{
		std::unique_ptr<Mesh> mesh;
	};

	//todo:或许需要使用多面对应不同的材质和贴图...
	using Meshes = std::vector<MeshData>;

private:

	/// <summary>
	/// ６つのメッシュにして、一つの面の処理を行うときやりやすくなる
	/// </summary>
	Meshes mMeshes; //描画

public:

	Cube();
	~Cube() = default;

	void Init(const char* fileName);

	void Init(std::shared_ptr<Texture>);

	/// @brief 描画
	void Draw(int texSlot = 0);

	void Update(float dt);


private:
	/// @brief MeshData(頂点)作成
	void CreateMeshes();

	/// @brief Default Shaderに使われるデータの書き込み
	const void WriteDefShader();

};

