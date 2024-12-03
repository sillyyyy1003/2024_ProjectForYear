#pragma once
#include "Primitive.h"
#include "Shader.h"
#include "Transform.h"

/// @brief �n�ʁE���ʁE���ȂǂɎg����
class Square :public Primitive
{
public:

	std::unique_ptr<Mesh> mMesh = nullptr;
public:

	Square();
	~Square() override = default;

	/// @brief Create 4 vertices square
	/// @param _fileName texture filepath
	/// @param _texUV sampler wrap UV
	void Init(const char* _fileName);

	void Init(const std::shared_ptr<Texture>& tex);

	/// @brief �����������ƍׂ�������
	/// @param _fileName 
	/// @param slices ������
	void Init(const char* _fileName, int slices = 0);

	/// @brief Create square with existing tex;
	/// @param slices
	///	@param tex
	void Init(const std::shared_ptr<Texture>& tex, int slices = 0);

	void Update(float dt);
	void Draw(int texSlot = 0);


	void SetScale(const DirectX::XMFLOAT2& scale) noexcept  override;

	/// @brief Square with 4 vertices
	void CreateMesh();

	/// @brief N x N�̏����������`�Ńv���[��
	/// @param slices ������
	void CreateMesh(UINT slices);


	/// @brief Default Shader�Ɏg����f�[�^�̏�������
	void WriteDefShader();
};

