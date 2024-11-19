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
	///	@param levels ���̐���������
	///	@param slices ���́E�V�����_�[����������
	void Init(const char* filePath, int levels = 64 , int slices = 64);

	/// @brief 
	/// @param tex 
	///	@param levels ���̐���������
	///	@param slices ���́E�V�����_�[����������
	void Init(std::shared_ptr<Texture>tex, int levels = 64, int slices = 64);
	

	void Update(float dt);
	void Draw(int texSlot);

private:

	/// @brief ���b�V���쐬
	///	@param levels ���̐���������
	///	@param slices ���́E�V�����_�[����������
	/// @param stacks �V�����_�[����������
	void CreateMesh(UINT levels, UINT slices, UINT stacks);

	void WriteDefShader();

};

