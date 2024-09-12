#pragma once
#include "Primitive.h"
#include "Shader.h"
#include "Transform.h"

/// @brief �n�ʁE���ʁE���ȂǂɎg����
class Plane3D :public Primitive
{
private:

	std::unique_ptr<Mesh> mMesh = nullptr;
	DirectX::XMFLOAT2 mTexUV = { 1,1 };

public:

	Plane3D();
	~Plane3D() = default;

	void Init(const char* _fileName);
	void Update(float dt);
	void Draw(int texSlot = 0);

	/// @brief Set Diffuse Color
	/// @param diffuse 
	void SetTexUV(DirectX::XMFLOAT2 _texUV = { 1,1 }) override;
	void SetScale(const DirectX::XMFLOAT2& scale) override;



private:
	const void CreateMeshes();
	const void CreateMaterial();
	const void CreateTexture(const char* _fileName);

	/*DefShader
	 * �g�p������
	 * �g�pDefault Camera
	 */
	/// @brief Default Shader�Ɏg����f�[�^�̏�������
	void WriteDefShader();
};

