#pragma once
#include "Primitive.h"
#include "Shader.h"
#include "Transform.h"

/// @brief �n�ʁE���ʁE���ȂǂɎg����
class Square :public Primitive
{
public:

	std::unique_ptr<Mesh> mMesh = nullptr;
	DirectX::XMFLOAT2 mTexUV = { 1,1 };


public:

	Square();
	~Square() override = default;

	/// @brief Create 4 vertices square with wrap UV
	/// @param _fileName texture filepath
	/// @param _texUV sampler wrap UV
	void Init(const char* _fileName, DirectX::XMFLOAT2 _texUV);

	/// @brief �����������ƍׂ�������
	/// @param _fileName 
	/// @param slices ������
	void Init(const char* _fileName, int slices);

	/// @brief Create square with existing;
	/// @param slices
	///	@param tex
	void Init(std::shared_ptr<Texture> tex, int slices);
	
	void Update(float dt);
	void Draw(int texSlot = 0);


	void SetTexUV(DirectX::XMFLOAT2 _texUV) noexcept override;
	void SetScale(const DirectX::XMFLOAT2& scale) noexcept  override;

	/// @brief Create the 4 vertices square with tex
	/// @param _fileName Tex file path
	void Init(const char* _fileName);

	/// @brief Square with 4 vertices
	void CreateMesh();

	/// @brief N x N�̏����������`�Ńv���[��
	/// @param slices ������
	void CreateMesh(UINT slices);


	/// @brief Default Shader�Ɏg����f�[�^�̏�������
	void WriteDefShader();
};
