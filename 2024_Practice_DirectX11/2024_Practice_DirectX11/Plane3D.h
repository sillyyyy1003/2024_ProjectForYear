#pragma once
#include "Primitive.h"
#include "Shader.h"
#include "Transform.h"

/// @brief 地面・平面・床などに使われる
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
	 * 使用方向光
	 * 使用Default Camera
	 */
	/// @brief Default Shaderに使われるデータの書き込み
	void WriteDefShader();
};

