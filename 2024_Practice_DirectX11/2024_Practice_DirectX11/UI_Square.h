#pragma once
#include "UI_Primitive.h"

class UI_Square :public UI_Primitive
{


public:

	UI_Square();
	~UI_Square()override = default;

	void Init(const char* _fileName = nullptr, int slices = 0, DirectX::XMINT2 _UVSplit = { 1,1 })override;
	void Init(const std::shared_ptr<Texture>& tex, int slices = 0, DirectX::XMINT2 _UVSplit = { 1,1 })override;
	//void Init(const std::shared_ptr<Texture>* tex, int slices = 0, DirectX::XMINT2 _split = { 1,1 })override;
	
	/// @brief Create Square Mesh with multi split
	/// @param slices 
	void CreateMesh(int slices);

	/// @brief Create Square with 4 vertices
	void CreateMesh();

	void Update(float dt);
	void Update();
	void Draw(int texSlot = 0);

	void WriteDefShader();

	void InitAnimation(DirectX::XMINT2 _split);
};

