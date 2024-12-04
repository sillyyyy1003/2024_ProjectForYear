#pragma once
#include "UI_Primitive.h"

class UI_RoundSquare:public UI_Primitive
{
protected:
	std::unique_ptr<Mesh> mMesh = nullptr;
	float mRoundRadius;	//円角の半径
	/// @brief
	/// mSquareSize.x
	/// mSquareSize.y 中心にある四角形のサイズ
	DirectX::XMFLOAT2 mSquareSize;
public:
	UI_RoundSquare();
	~UI_RoundSquare();

	void Init(const std::shared_ptr<Texture>& pTex, DirectX::XMFLOAT2 _squareSize, float radius, int slices, DirectX::XMINT2 _UVSplit);

	void Update() override;
	void Draw(int texSlot=0) override;

protected:
	void CreateMesh(int slices) override;
	void WriteDefShader() override;
};

