#pragma once
#include "Primitive.h"

//円形の平面　容器内の平面用
class Circle :public Primitive
{
protected:

	std::shared_ptr<Mesh> mMesh = nullptr;
	
public:

	Circle();
	~Circle() = default;

	void Update(float dt);
	void Draw(int texSlot = 0);

	void SetTexUV(DirectX::XMFLOAT2 _texUV)  noexcept override;


	void Init(const char* _fileName, int slices = 64);
	void Init(const std::shared_ptr<Texture>& tex, int slices = 64);



	//分割が少ないメッシュを作る
	void CreateMesh(UINT slices);

	//球体のような複雑なメッシュを作る
	void CreateMesh(UINT levels, UINT slices);


	 /// @brief Default Shaderに使われるデータの書き込み
	void WriteDefShader();

	std::shared_ptr<Mesh> GetMesh() { return mMesh; };
};

