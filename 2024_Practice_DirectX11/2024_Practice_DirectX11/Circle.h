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

	void SetTexUV(DirectX::XMFLOAT2 _texUV)  noexcept override;
	void SetScale(const DirectX::XMFLOAT2& scale) noexcept override;
	void SetScale(const float* scale)  noexcept override;


	void Init(const char* _fileName);

	//���������Ȃ����b�V�������
	void CreateMesh(UINT slices);

	//���̂̂悤�ȕ��G�ȃ��b�V�������
	void CreateMesh(UINT levels, UINT slices);
	void CreateMaterial();
	void CreateTexture(const char* _fileName);


	 /// @brief Default Shader�Ɏg����f�[�^�̏�������
	void WriteDefShader();
};

