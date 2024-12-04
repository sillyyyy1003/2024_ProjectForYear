#pragma once
#include "Primitive.h"
#include "Shader.h"
#include "Transform.h"
#include "UVAnimation.h"

/// @brief �n�ʁE���ʁE���ȂǂɎg����
class Square :public Primitive
{
public:

	std::unique_ptr <Mesh> mMesh = nullptr;
public:

	Square();
	~Square() override = default;

	/// @brief Create 4 vertices square
	/// @param _fileName texture filepath
	/// @param _texUV sampler wrap UV
	void Init(const char* _fileName, DirectX::XMINT2 _UVSplit = { 1,1 }) override;

	void Init(const std::shared_ptr<Texture>& tex, DirectX::XMINT2 _UVSplit = { 1,1 }) override;

	/// @brief �����������ƍׂ�������
	/// @param _fileName 
	/// @param slices ������
	///
	void Init(const char* _fileName, int slices = 0, DirectX::XMINT2 _UVSplit = { 1,1 }) override;

	/// @brief Create square with existing tex;
	/// @param slices
	///	@param tex
	void Init(const std::shared_ptr<Texture>& tex, int slices = 0, DirectX::XMINT2 _UVSplit = { 1,1 }) override;

	void Update(float dt) override;
	void Draw(int texSlot = 0)override;


	void SetScale(const DirectX::XMFLOAT2& scale) noexcept  override;

	/// @brief Square with 4 vertices
	void CreateMesh();

	/// @brief N x N�̏����������`�Ńv���[��
	/// @param slices ������
	void CreateMesh(UINT slices) override;


	/// @brief Default Shader�Ɏg����f�[�^�̏�������
	void WriteDefShader() override;
};

