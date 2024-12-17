#pragma once
#include "Primitive.h"

//�~�`�̕��ʁ@�e����̕��ʗp
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


	//���������Ȃ����b�V�������
	void CreateMesh(UINT slices)override;

	//���̂̂悤�ȕ��G�ȃ��b�V�������
	void CreateMesh(UINT levels, UINT slices)override;


	 /// @brief Default Shader�Ɏg����f�[�^�̏�������
	void WriteDefShader()override;

	Mesh* GetMesh() const override { return mMesh.get(); };
};

