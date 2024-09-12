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
	VertexShader* mVS = nullptr;
	PixelShader* mPS = nullptr;

	std::shared_ptr<VertexShader> mDefVS = nullptr;
	std::shared_ptr<PixelShader> mDefPS = nullptr;



public:

	Cube();
	~Cube() = default;

	void Init(const char* fileName = nullptr);

	/// @brief 描画
	void Draw(int texSlot = 0);

	void Update(float dt);


private:
	/// @brief MeshData(頂点)作成
	const void CreateMeshes();

	/// @brief Materialを作る default Color:White
	/// @param matNum materialの数
	const void CreateMaterial(int matNum = 6);

	/// @brief ファイルを読み込んでテクスチャを作る
	/// @param fileName ファイル名(テクスチャなしの状態もあり)
	const void CreateTexture(const char* fileName);

	/*DefShader 
	 * 使用方向光
	 * 使用Default Camera
	 */
	/// @brief Default Shaderに使われるデータの書き込み
	const void WriteDefShader();

};

