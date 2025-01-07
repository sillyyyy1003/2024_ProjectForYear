#pragma once
#include "Circle.h"
#include "Square.h"
#include "WaterState.h"
#include "FastNoise/FastNoiseLite.h"

//Water Default Parameter
namespace WaterDefault
{
	//default water material
	const Material defaultMat =
	{
		Color(0.8f, 0.8f, 0.8f, 1.0f),
		Color(0.3f, 0.3f, 0.3f, 0.3f),
		Color(0.8f, 0.8f, 0.8f, 32.0f),
		Color(0.0f, 0.0f, 0.0f, 1.0f),
	};

	//Default circle slice
	constexpr int DEFAULT_SLICES = 64;
	//Default circle level
	constexpr int DEFAULT_LEVELS = 64;
}

/// @brief ポーションの規定になる
class Water
{
private:
	bool isResetVertices = false;	//すべての頂点の位置をゼロにするかどうか？
protected:

	WaterStateConfig::WaterStateParam mParam = {};

	//波紋開始かどうか？
	bool isTrigger = true;
	float mNowAmplitude = 0.0f;
	float mWaterTime = 0.0;		//今の波の時間
	float mDuration =3.0f;		// 波の継続時間
	WaterStateConfig::WaterBoilingState mBoilingState;//動きがあるかどうかの判断

	std::unique_ptr<WaterState> mWaterStates = nullptr;


	//水の描画 //todo: make the model multi kinds
	std::unique_ptr<Primitive> mModel = nullptr;

	std::string mObjectName;
	bool isApplyParam = false;
	
public:
	
	Water();
	virtual ~Water() = default;

	/// @brief Init object & creating new texture
	/// @param filePath water texture filepath
	/// @param objName オブジェクト名
	///	@param slices 分割数
	void Init(const char* filePath, const char* objName, int slices = WaterDefault::DEFAULT_SLICES);
		
	/// @brief Init Object with existing texture
	/// @param tex 既にロードされたテクスチャ
	/// @param objName オブジェクト名
	///	@param slices 分割数
	void Init(const std::shared_ptr<Texture>& tex, const char* objName, int slices = WaterDefault::DEFAULT_SLICES);


	virtual void Update(float dt);

	/// @brief アップデートの最後で行う 描画の頂点の更新とシェーダにデータ書き込む
	void RenderUpdate();

	void Draw();

	/// @brief WaterParamの内容をシェーダに書き込む
	void WriteShader();

	void LoadDefShader(const std::shared_ptr<VertexShader>& vs, const std::shared_ptr<PixelShader>& ps);
	void LoadShader(const std::shared_ptr<VertexShader>& vs, const std::shared_ptr<PixelShader>& ps);
	void LoadDefShader(const char* vsPath, const char* psPath);

	/// @brief Load Save Data & Init Object Data
	/// @param data json fileData
	/// @param objName dataName
	void LoadSaveData(json data, const char* objName);

	void UpdateColor(DirectX::XMFLOAT4 color);

	/// @brief オブジェクトのデータをjsonファイルに書き込み
	/// @return 
	json SaveData();

	void SetTexture(std::shared_ptr<Texture> tex);

	void RewriteVertices();

	void SetWaterBoilingState(WaterStateConfig::WaterBoilingState _state);
	void SetWaterState(WaterStateConfig::WaterState _state);

	float GetRadius() { return mModel->GetScale().x / 2.f; };
	DirectX::XMFLOAT3 GetPosition() { return mModel->GetPosition(); };


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

	void ResetMaterial();

	/// @brief 後処理(Shaderに書き込み)
	/// @param dt delta Time
	void LateUpdate(float dt);
};

