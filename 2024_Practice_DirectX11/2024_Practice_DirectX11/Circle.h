#pragma once
#include "Primitive.h"

//�~�`�̕��ʁ@�e����̕��ʗp
class Circle :public Primitive
{
private:

	std::unique_ptr<Mesh> mMesh = nullptr;
	DirectX::XMFLOAT2 mTexUV = { 1,1 };

public:

	Circle();
	~Circle() = default;

	void Update(float dt);
	void Draw(int texSlot = 0);

	/// @brief Set Diffuse Color
	/// @param diffuse 
	void SetTexUV(DirectX::XMFLOAT2 _texUV) override;
	void SetScale(const DirectX::XMFLOAT2& scale) override;


protected:
	void Init(const char* _fileName);
	const void CreateMeshes(UINT slices);
	const void CreateMaterial();
	const void CreateTexture(const char* _fileName);


	 /// @brief Default Shader�Ɏg����f�[�^�̏�������
	void WriteDefShader();
};

