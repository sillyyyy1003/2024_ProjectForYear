#pragma once
#include "Primitive.h"
#include "Shader.h"
#include "Transform.h"

/// @brief 地面・平面・床などに使われる
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

	/// @brief 分割がもっと細かい平面
	/// @param _fileName 
	/// @param slices 分割数
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

	/// @brief N x Nの小さい正方形でプレーン
	/// @param slices 分割数
	void CreateMesh(UINT slices);


	/// @brief Default Shaderに使われるデータの書き込み
	void WriteDefShader();
};

