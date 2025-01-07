#pragma once
#include "Primitive.h"

//円形の平面　容器内の平面用
class Circle :public Primitive
{
protected:

	std::unique_ptr<Mesh> mMesh = nullptr;
	
public:

	Circle();
	~Circle() = default;

	void Update(float dt) override;
	void Draw(int texSlot = 0) override;

	void Init(const char* _fileName, int slices = 64, DirectX::XMINT2 _UVSplit = { 1,1 })override;
	void Init(const char* _fileName, int slices = 64, int levels = 64, DirectX::XMINT2 _UVSplit = { 1,1 })override;


	void Init(const std::shared_ptr<Texture>& tex, int slices = 64, DirectX::XMINT2 _UVSplit = { 1,1 })override;
	void Init(const std::shared_ptr<Texture>& tex, int slices = 64, int levels = 64, DirectX::XMINT2 _UVSplit = { 1,1 })override;


	//分割が少ないメッシュを作る
	void CreateMesh(UINT slices)override;

	//球体のような複雑なメッシュを作る
	void CreateMesh(UINT levels, UINT slices)override;


	 /// @brief Default Shaderに使われるデータの書き込み
	void WriteDefShader()override;

	Mesh* GetMesh() const override { return mMesh.get(); };
};

