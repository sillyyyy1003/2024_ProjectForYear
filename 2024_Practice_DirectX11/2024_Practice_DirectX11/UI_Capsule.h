#pragma once
#include "Primitive.h"
#include "UI_Primitive.h"


class UI_Capsule : public UI_Primitive
{
protected:
	std::unique_ptr<Mesh> mMesh = nullptr;

	DirectX::XMFLOAT2 mSquareSize = { 1,1 };

public:
	UI_Capsule();
	~UI_Capsule() override=default;

	void Init(const std::shared_ptr<Texture>& tex, DirectX::XMFLOAT2 _squareSize, int slices = UIPrimitiveConfig::defaultSlices, DirectX::XMINT2 _split = { 1,1 });
	void Init(const char* filePath, DirectX::XMFLOAT2 _squareSize, int slices = UIPrimitiveConfig::defaultSlices, DirectX::XMINT2 _split = { 1,1 });

	void Update();
	void Draw(int texSlot = 0);

	/// @brief Set Capsule square length& circle radius
	/// @param size 
	void SetScale(const DirectX::XMFLOAT2& size);

	/// @brief ���䗦�g��k��
	/// @param scale 
	void SetScale(float scale);

	
protected:

	void CreateMesh(int slices);

	/// @brief Default Shader�Ɏg����f�[�^�̏�������
	void WriteDefShader();

	std::vector<Vertex::VtxPosNormalTex> CreateNewVertexData(int slices=UIPrimitiveConfig::defaultSlices);
};
