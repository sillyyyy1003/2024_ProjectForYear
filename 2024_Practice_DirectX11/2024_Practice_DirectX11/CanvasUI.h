#pragma once
#include "Collider.h"
#include "Primitive.h"
#include "Transform.h"

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

	struct MaterialData
	{
		Material material;
		std::unique_ptr<Texture> tex;
	};

	//Material & Texture
	MaterialData mMaterial = {};

	//Mesh
	std::unique_ptr<Mesh> mMesh = {};

	//VertexShader & PixelShader
	std::unique_ptr<VertexShader> mDefVS = nullptr;	// Default Vertex Shader
	std::unique_ptr<PixelShader> mDefPS = nullptr;	// Default Pixel Shader
	VertexShader* mVS = nullptr;
	PixelShader* mPS = nullptr;

	//最初の大きさを記録し、viewport変化するときに大きさを変化しないように
	DirectX::XMFLOAT2 mOriginScale = {};
	DirectX::XMFLOAT2 mOriginPos = {};

	bool isDefShader = true;

	std::string mFilePath= {};
	std::string mObjectName = {};
public:
	CanvasUI() = default;
	virtual ~CanvasUI();

	/// @brief UIのテクスチャとシェーダの初期化
	///	基本UI/No Lighting Effect
	/// @param _fileName テクスチャファイル名
	virtual void Init(const char* _fileName = nullptr);

	/// @brief 位置の初期化
	/// @param pos ピクセル位置
	virtual void InitPosition(DirectX::XMFLOAT3 pos);

	/// @brief Load Save Data & Init Object Data
	/// @param data json fileData
	/// @param objName dataName
	virtual void LoadSaveData(json data, const char* objName);

	virtual void Update(float dt);

	virtual void Draw();

	/// @brief Canvasのサイズを設定
	/// @param x 幅
	/// @param y 高
	void SetSize(float x, float y);

	/// @brief 自動並び替え描画
	/// @param z 
	void SetPosZ(float z);


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

protected:

	/// @brief メッシュ作成
	void CreateMeshBuffer();

	/// @brief テクスチャ作成
	/// @param _fileName テクスチャファイル名
	void CreateTexture(const char* _fileName);

	/// @brief マテリアル作成
	/// @param _fileName テクスチャファイル名
	void CreateMaterial(const char* _fileName);

	virtual void LoadShader();

	/// @brief uiのスケールを計算して、uiの大きさを保つ
	/// @return viewWidth&viewHeightの計算
	void UpdateScale();

	void UpdatePos();

	/// @brief オブジェクト状態をシェーダに書き込み
	virtual void WriteShader();

};

