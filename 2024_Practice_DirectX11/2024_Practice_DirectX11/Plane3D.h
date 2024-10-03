#pragma once
#include "Primitive.h"
#include "Shader.h"
#include "Transform.h"

/// @brief �n�ʁE���ʁE���ȂǂɎg����
class Plane3D :public Primitive
{
public:

	std::unique_ptr<Mesh> mMesh = nullptr;
	DirectX::XMFLOAT2 mTexUV = { 1,1 };

public:

	Plane3D();
	~Plane3D() = default;

	/// @brief Plane�p�������v���O����
	/// @param _fileName texture filepath
	/// @param _texUV sampler wrap UV
	void Init(const char* _fileName, DirectX::XMFLOAT2 _texUV) override;

	/// @brief �����������ƍׂ�������
	/// @param _fileName 
	/// @param slices ������
	void Init(const char* _fileName, int slices);
	
	void Update(float dt);
	void Draw(int texSlot = 0);


	void SetTexUV(DirectX::XMFLOAT2 _texUV) noexcept override;
	void SetScale(const DirectX::XMFLOAT2& scale) noexcept  override;


	void Init(const char* _fileName);
	const void CreateMesh();
	/// @brief N x N�̏����������`�Ńv���[��
	/// @param slices ������
	const void CreateMesh(UINT slices);

	const void CreateMaterial();
	const void CreateTexture(const char* _fileName);



	/*DefShader
	 * �g�p������
	 * �g�pDefault Camera
	 */
	/// @brief Default Shader�Ɏg����f�[�^�̏�������
	void WriteDefShader();
};

