#pragma once
#include "Primitive.h"
class Capsule :public Primitive
{
private:

	std::unique_ptr<Mesh> mMesh = nullptr;

public:
	Capsule();
	~Capsule() override = default;

	void Init(const char* filePath);
	void Update(float dt);
	void Draw(int texSlot);

private:

	/// @brief メッシュ作成
	///	@param levels 球体垂直分割数
	///	@param slices 球体・シリンダー水平分割数
	/// @param stacks シリンダー垂直分割数
	void CreateMesh(UINT levels, UINT slices, UINT stacks);

	void CreateMaterial();
	void CreateTexture(const char* filePath);

	void WriteDefShader();

};

