#pragma once
#include <unordered_map>

#include "CameraBase.h"
#include "SceneBase.h"
#include "Shader.h"
#include "Texture.h"

namespace SceneConfig
{
	enum class SceneIndex:unsigned int
	{
		//This is the first class of scenes
		SCENE_TITLE,
		SCENE_OPTION,
		SCENE_LAB,
		EXIT,

		SCENE_MISSION,
		SCENE_POTION,
		SCENE_IN_GAME_OPTION,
		SCENE_MAX,
	};

	
}


/// @brief すべてのシーンの管理 //在这里完成GameScene的SubScene分发
class SceneManager :public SceneBase
{
private:

	SceneConfig::SceneIndex mSceneIndex;//Scene Index
	std::unordered_map<std::string, SceneConfig::SceneIndex> mSceneMap;
	bool isChangeMainScene = false;

	std::shared_ptr<CameraBase> mCurrentCamera;
	bool isSwitchScene = false;//if Switch Scene

	std::unordered_map<std::string, std::shared_ptr<Texture>> mFontLib;


protected:

	std::shared_ptr<Texture> UIFont_StringLiteral;
	std::shared_ptr<Texture> UIFont_OCRA_Extend;
	std::shared_ptr<Texture> UIFont_Courier_New_Regular;
	std::shared_ptr<Texture> UIFont_Courier_New_Bold;
	std::shared_ptr<Texture> UIFont_Courier_New_It;

	std::shared_ptr<Texture> UIFont_Source_Code_Pro_It;
	
	//These are resources & data for the sub scene
	std::shared_ptr<Texture> pbrAlbedoTexture;
	std::shared_ptr<Texture> pbrNormalTexture;
	std::shared_ptr<Texture> pbrMetallicTexture;
	std::shared_ptr<Texture> pbrAOTexture;
	//PlayerData

	//TestData
	std::shared_ptr<Texture> waterTexture;
	std::shared_ptr<Texture> paperTexture;

private:

	SceneManager() {};
	~SceneManager() = default;

public:

	static SceneManager* Get() { static SceneManager instance; return &instance; };

	/// @brief 現在シーンのカメラ取得
	/// @return 
	CameraBase* GetCurrentCamera() const { return mCurrentCamera.get(); };
	/// @brief シーンの切り替えと共に現在のカメラを取得
	/// @param camera 
	void SetCurrentCamera(std::shared_ptr<CameraBase> camera);

	/// @brief 共通オブジェクトの読み込み・作成
	void Init();

	/// @brief データ保存・リソースリリース
	void UnInit();

	/// @brief ここでゲーム全体の時間処理などを行う
	/// @param dt deltaTime
	void Update(float dt);

	/// @brief 描画
	void Draw();

	

	//================================
	// シーン切り替え関連
	//================================

	SceneConfig::SceneIndex GetCurrentScene() { return mSceneIndex; };

	/// @brief 現在シーン切り替えしているか？
	/// @return 
	bool GetSwitchScene() { return isSwitchScene; };

	/// @brief シーンの切り替えを設定する
	/// @param isSwitchScene 
	void SetSwitchScene(bool isSwitchScene) { this->isSwitchScene = isSwitchScene; };

	void SetMainScene(const std::string& sceneName);




private:

	/// @brief 使うシェーダーファイルを事前に読み込み
	void LoadPixelShaderFile();

	void LoadVertexShaderFile();

	void ChangeMainScene();

	void ClearMainSceneChange();

	void MainSceneChangeListener();

	/// @brief Init Scene Map, Fill all the scene config into the map
	void InitSceneMap();

	/// @brief FontLibの初期化を行う
	void InitFontLib();

	void InitModelTexture();
};

