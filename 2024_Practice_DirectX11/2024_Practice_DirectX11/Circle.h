#pragma once
#include "Primitive.h"

//�~�`�̕��ʁ@�e����̕��ʗp
class Circle :public Primitive
{
protected:

	std::shared_ptr<Mesh> mMesh = nullptr;
	
public:

	Circle();
	~Circle() = default;

	void Update(float dt);
	void Draw(int texSlot = 0);

	void SetTexUV(DirectX::XMFLOAT2 _texUV)  noexcept override;


	void Init(const char* _fileName, int slices = 64);
	void Init(const std::shared_ptr<Texture>& tex, int slices = 64);



	//���������Ȃ����b�V�������
	void CreateMesh(UINT slices);

	//���̂̂悤�ȕ��G�ȃ��b�V�������
	void CreateMesh(UINT levels, UINT slices);


	 /// @brief Default Shader�Ɏg����f�[�^�̏�������
	void WriteDefShader();

	std::shared_ptr<Mesh> GetMesh() { return mMesh; };
};

