#pragma once
#include "Circle.h"
#include "Plane3D.h"


class Water
{


	struct WaterParam
	{
		DirectX::XMFLOAT3  center;  // 波の中心位置
		float amplitude;			// 波の幅
		float frequency;			// 波の頻度
		float speed;				// 波のスビード
		float sigma;				// 波の拡散範囲
		float time;					// 経過時間
	};

	WaterParam mParam = {};
	//std::unique_ptr<Plane3D> mModel;

	std::unique_ptr<Circle> mModel;
	//波紋開始かどうか？
	bool isTrigger = true;
	float duration = 3.0f;


public:
	
	Water();
	~Water() = default;

	void Init(const char* filePath);
	void Update(float dt);
	void Draw();

	/// @brief WaterParamの内容をシェーダに書き込む
	void WriteShader();

	//Plane3D* GetModel() { return mModel.get(); };
	Circle* GetModel() { return mModel.get(); };
};

