#pragma once
#include "Primitive.h"
#include "Transform.h"
#include "UVAnimation.h"

class PixelShader;
class VertexShader;
class Texture;

/// <summary>
/// Using D3D Render 
/// </summary>
class CanvasUI
{
public:

	Transform mTransform = {};
	

protected:
	//Material & Texture
	MaterialData mMaterial = {};

	//Mesh
	std::unique_ptr<Mesh> mMesh = {};

	//VertexShader & PixelShader
	std::shared_ptr<VertexShader> mDefVS = nullptr;	// Default Vertex Shader
	std::shared_ptr<PixelShader> mDefPS = nullptr;	// Default Pixel Shader
	VertexShader* mVS = nullptr;
	PixelShader* mPS = nullptr;

	//最初の大きさを記録し、viewport変化するときに大きさを変化しないように
	DirectX::XMFLOAT2 mOriginScale = {};
	DirectX::XMFLOAT2 mOriginPos = {};

	bool isDefShader = true;
	bool isUseUVAnimation = false;//UVAnimationを使うか

	std::string mFilePath= {};
	std::string mObjectName = {};

	std::unique_ptr<UVAnimation> mUvAnimation = nullptr;
public:
	CanvasUI() = default;
	virtual ~CanvasUI();

	/// @brief UIのテクスチャとシェーダの初期化
	///	基本UI/No Lighting Effect
	/// @param _fileName テクスチャファイル名
	///	@param _split 分割
	virtual void Init(const char* _fileName = nullptr,DirectX::XMINT2 _split={1,1});

	virtual void Init(MaterialData& _materialData, DirectX::XMINT2 _split = { 1,1 });

	virtual void Init(std::shared_ptr<Texture> tex, DirectX::XMINT2 _split = { 1,1 });

	/// @brief 位置の初期化
	/// @param pos ピクセル位置
	virtual void SetPosition(DirectX::XMFLOAT3 pos);

	/// @brief Load Save Data & Init Object Data
	/// @param data json fileData
	/// @param objName dataName
	virtual void LoadSaveData(json data, const char* objName);

	virtual void Update(float dt);
	virtual void Update();


	virtual void Draw();

	/// @brief Canvasのサイズを設定
	/// @param width 幅
	/// @param height 高
	void SetCanvasSize(float width, float height);

	/// @brief 自動並び替え描画
	/// @param z 
	void SetPosZ(float z);


	/// @brief Set Canvas position
	/// @param x 
	/// @param y 
	void SetPosition(float x, float y);


	/// @brief テクスチャのファイルパスを取得
	/// @return ファイルパス
	std::string GetFilePath() const { return mFilePath; };

	std::string GetObjectName() const { return mObjectName; };

	/// @brief 位置を取得
	/// @return 
	DirectX::XMFLOAT3 GetPosition() const { return mTransform.GetPosition(); };

	/// @brief 大きさを取得
	/// @return X:幅　Y:高さ
	DirectX::XMFLOAT2 GetScaleXY() const { return DirectX::XMFLOAT2{ mTransform.GetPosition().x,mTransform.GetPosition().y }; };

	DirectX::XMFLOAT3 GetScale()const { return mTransform.GetScale(); };
	DirectX::XMFLOAT4 GetQuaternion()const { return mTransform.GetQuaternion(); };
	DirectX::XMFLOAT3 GetRotation()const { return mTransform.GetRotation(); };

	const Material& GetMaterial() { return mMaterial.material; };

	virtual json SaveData(const char* objName);

	UVAnimation* GetUVAnimation() { return mUvAnimation.get(); };

	const DirectX::XMFLOAT4& GetDiffuseColor() { return mMaterial.material.diffuse; };

	/// @brief 色設定
	/// @param color 
	void SetColor(DirectX::XMFLOAT4 color);

	/// @brief 透明度設定
	/// @param _transparency 
	void SetTransparency(float _transparency);

	void LoadPSShader(const char* filePath);
protected:

	/// @brief メッシュ作成
	void CreateMeshBuffer();

	/// @brief テクスチャ作成
	/// @param _fileName テクスチャファイル名
	void CreateTexture(const char* _fileName);

	/// @brief テクスチャ作成
	/// @param pTex 作成されているTexture pointer
	void CreateTexture(const std::shared_ptr<Texture>& pTex);

	/// @brief マテリアル作成
	/// @param _fileName テクスチャファイル名
	void CreateMaterial(const char* _fileName);

	/// @brief マテリアル作成
	/// @param _materialData 作成されているマテリアルデータ
	void CreateMaterial(MaterialData& _materialData);

	void CreateMaterial(const std::shared_ptr<Texture>& tex);

	/// @brief Load Default Shader
	virtual void LoadShader();



	/// @brief シェーダファイルをリセット
	/// @param PSFile PS file path
	/// @param VSFile VS file path
	virtual void SetShader(const char* PSFile, const char* VSFile);

	/// @brief uiのスケールを計算して、uiの大きさを保つ
	/// @return viewWidth&viewHeightの計算
	void UpdateScale();

	void UpdatePos();

	/// @brief オブジェクト状態をシェーダに書き込み
	virtual void WriteShader();

};

