#pragma once
#include "Primitive.h"

//円形の平面　容器内の平面用
class Circle :public Primitive
{
public:

	std::unique_ptr<Mesh> mMesh = nullptr;

public:

	Circle();
	~Circle() = default;

	void Update(float dt);
	void Draw(int texSlot = 0);

	void SetTexUV(DirectX::XMFLOAT2 _texUV)  noexcept override;
	void SetScale(const DirectX::XMFLOAT2& scale) noexcept override;
	void SetScale(const float* scale)  noexcept override;


	void Init(const char* _fileName);

	//分割が少ないメッシュを作る
	void CreateMesh(UINT slices);

	//球体のような複雑なメッシュを作る
	void CreateMesh(UINT levels, UINT slices);
	void CreateMaterial();
	void CreateTexture(const char* _fileName);


	 /// @brief Default Shaderに使われるデータの書き込み
	void WriteDefShader();
};

