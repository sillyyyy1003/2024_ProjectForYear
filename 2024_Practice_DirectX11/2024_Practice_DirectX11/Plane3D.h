#pragma once
#include "Primitive.h"
#include "Shader.h"
#include "Transform.h"

/// @brief 地面・平面・床などに使われる
class Plane3D :public Primitive
{
public:

	std::unique_ptr<Mesh> mMesh = nullptr;
	DirectX::XMFLOAT2 mTexUV = { 1,1 };

public:

	Plane3D();
	~Plane3D() = default;

	/// @brief Plane用初期化プログラム
	/// @param _fileName texture filepath
	/// @param _texUV sampler wrap UV
	void Init(const char* _fileName, DirectX::XMFLOAT2 _texUV) override;

	/// @brief 分割がもっと細かい平面
	/// @param _fileName 
	/// @param slices 分割数
	void Init(const char* _fileName, int slices);
	
	void Update(float dt);
	void Draw(int texSlot = 0);


	void SetTexUV(DirectX::XMFLOAT2 _texUV) noexcept override;
	void SetScale(const DirectX::XMFLOAT2& scale) noexcept  override;


	void Init(const char* _fileName);
	const void CreateMesh();
	/// @brief N x Nの小さい正方形でプレーン
	/// @param slices 分割数
	const void CreateMesh(UINT slices);

	const void CreateMaterial();
	const void CreateTexture(const char* _fileName);



	/*DefShader
	 * 使用方向光
	 * 使用Default Camera
	 */
	/// @brief Default Shaderに使われるデータの書き込み
	void WriteDefShader();
};

