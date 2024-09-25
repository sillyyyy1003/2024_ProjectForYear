#pragma once
#include "Circle.h"
#include "Plane3D.h"

/// @brief ポーションの規定になる
///	todo:今は円形だけの対応
class Water
{
private:

	const Material defaultMat = 
	{
		Color(0.8f, 0.8f, 0.8f, 1.0f),	
		Color(0.2f, 0.3f, 0.6f, 0.3f),	
		Color(0.8f, 0.8f, 0.8f, 32.0f),	
		Color(0.0f, 0.0f, 0.0f, 1.0f),	
	};

protected:

	//水の物理表現に関するパラメータ
	struct WaterParam
	{
		DirectX::XMFLOAT3  center;  // 波の中心位置
		float nowAmplitude;			// 波の幅
		float amplitude;

		float frequency;			// 波の頻度
		float speed;				// 波のスビード
		float sigma;				// 波の拡散範囲
		float time = 0;					// 経過時間
	};

	WaterParam mParam = {};

	//波紋開始かどうか？
	bool isTrigger = true;
	float mDuration = 3.0f;		// 波の継続時間

	//水の描画
	std::unique_ptr<Circle> mModel;//todo:プレーン多種対応にする

public:
	
	Water();
	virtual ~Water() = default;

	void Init(const char* filePath);

	void Update(float dt);

	/// @brief アップデートの最後で行う 描画の頂点の更新とシェーダにデータ書き込む
	void RenderUpdate();

	void Draw();

	/// @brief WaterParamの内容をシェーダに書き込む
	void WriteShader();

	Circle* GetModel() { return mModel.get(); };

	/// @brief 波の中心点を設定する
	/// @param centerPos 波の中心点
	void SetCenter(DirectX::XMFLOAT3 centerPos);

	/// @brief 波の頻度を設定する
	/// @param frequency 波の頻度
	void SetFrequency(float frequency);

	/// @brief 波のスビードを設定する
	/// @param speed 波のスビード
	void SetSpeed(float speed);

	/// @brief 波の幅を設定する
	/// @param amplitude 波の幅
	void SetAmplitude(float amplitude);

	/// @brief 波の継続時間を設定する
	/// @param duration 波の継続時間
	void SetWaveDuration(float duration);

	/// @brief Load Save Data & Init Object Data
	/// @param data json fileData
	/// @param objName dataName
	virtual void LoadSaveData(json data, const char* objName);

	/// @brief オブジェクトのデータをjsonファイルに書き込み
	/// @return 
	virtual json SaveData();
	
	void ResetMaterial();

protected:

	/// @brief 後処理(Shaderに書き込み)
	/// @param dt delta Time
	void LateUpdate(float dt);
};

