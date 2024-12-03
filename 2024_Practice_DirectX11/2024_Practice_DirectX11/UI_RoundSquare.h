#pragma once
#include "UI_Primitive.h"

class UI_RoundSquare:public UI_Primitive
{
protected:
	std::unique_ptr<Mesh> mMesh = nullptr;
	float mRoundRadius;	//â~äpÇÃîºåa
	/// @brief
	/// mSquareSize.x
	/// mSquareSize.y íÜêSÇ…Ç†ÇÈéläpå`ÇÃÉTÉCÉY
	DirectX::XMFLOAT2 mSquareSize;
public:
	UI_RoundSquare();
	~UI_RoundSquare();

	void Init(const std::shared_ptr<Texture>& pTex, DirectX::XMFLOAT2 _squareSize, float radius, int slices, DirectX::XMINT2 _UVSplit);

	void Update() override;
	void Draw(int texSlot) override;

protected:
	void CreateMesh() override;
	void WriteDefShader() override;
};

