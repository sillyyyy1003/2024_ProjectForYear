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

	/// @brief ���b�V���쐬
	///	@param levels ���̐���������
	///	@param slices ���́E�V�����_�[����������
	/// @param stacks �V�����_�[����������
	void CreateMesh(UINT levels, UINT slices, UINT stacks);

	void CreateMaterial();
	void CreateTexture(const char* filePath);

	void WriteDefShader();

};

