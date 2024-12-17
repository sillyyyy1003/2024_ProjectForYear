#pragma once
#include "UI_Primitive.h"
class UISquare : public UI_Primitive
{

public:

	UISquare();
	~UISquare() override = default;

	void Init(const std::shared_ptr<Texture>& tex, DirectX::XMINT2 _UVSplit);

	/// @brief Create Square with 4 vertices
	void CreateMesh() override;

	void Update() override;
	void Draw(int texSlot = 0) override;
	void WriteDefShader() override;

	json SaveData();
	void LoadSaveData(json data);

	
};

