#pragma once
#include "Primitive.h"

//�~�`�̕��ʁ@�e����̕��ʗp
class Circle :public Primitive
{
public:

	std::unique_ptr<Mesh> mMesh = nullptr;

public:

	Circle();
	~Circle() = default;

	void Update(float dt);
	void Draw(int texSlot = 0);

	void SetTexUV(DirectX::XMFLOAT2 _texUV) override;
	void SetScale(const DirectX::XMFLOAT2& scale) override;
	void SetScale(const float* scale) override;


	void Init(const char* _fileName);

	//���������Ȃ����b�V�������
	const void CreateMesh(UINT slices);

	//���̂̂悤�ȕ��G�ȃ��b�V�������
	const void CreateMesh(UINT levels, UINT slices);
	const void CreateMaterial();
	const void CreateTexture(const char* _fileName);


	 /// @brief Default Shader�Ɏg����f�[�^�̏�������
	void WriteDefShader();
};

