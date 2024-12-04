#pragma once
#include "Primitive.h"
#include "UI_Primitive.h"

//todo: need to fix uv animation problem which is not implement in the UI_Capsule
class UI_Capsule : public UI_Primitive
{
protected:
	std::unique_ptr<Mesh> mMesh = nullptr;
	DirectX::XMFLOAT2 mSquareSize = { 1,1 };

public:
	UI_Capsule();
	~UI_Capsule() override=default;

	void Init(const std::shared_ptr<Texture>& tex, DirectX::XMFLOAT2 _squareSize, int slices = UIPrimitiveConfig::defaultSlices, DirectX::XMINT2 _split = { 1,1 })override;
	void Init(const char* filePath, DirectX::XMFLOAT2 _squareSize, int slices = UIPrimitiveConfig::defaultSlices, DirectX::XMINT2 _split = { 1,1 })override;

	void Update()override;
	void Draw(int texSlot = 0)override;

	/// @brief Set Capsule square length& circle radius
	/// @param size 
	void SetScale(const DirectX::XMFLOAT2& size) override;

	/// @brief 等比率拡大縮小
	/// @param scale 
	void SetScale(float scale) override;

	
protected:

	void CreateMesh(int slices) override;

	/// @brief Default Shaderに使われるデータの書き込み
	void WriteDefShader() override;

	std::vector<Vertex::VtxPosNormalTex> CreateNewVertexData(int slices=UIPrimitiveConfig::defaultSlices);
};

