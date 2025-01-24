#pragma once
#include <unordered_map>

#include "CameraBase.h"
#include "Player.h"
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
		SCENE_EXIT,

		SCENE_MISSION,
		SCENE_POTION,
		SCENE_IN_GAME_OPTION,
		SCENE_MAX,

		SCENE_NONE
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
	//Font Texture
	std::shared_ptr<Texture> UIFont_OCRA_Extend;

	//Texture
	std::shared_ptr<Texture> fantasyHousePropsAlbedoTexture;
	std::shared_ptr<Texture> fantasyHousePropsNormalTexture;
	std::shared_ptr<Texture> fantasyHousePropsMetallicTexture;

	std::shared_ptr<Texture> waterTexture;
	std::shared_ptr<Texture> paperTexture;

	std::shared_ptr<Texture> tableTexture;
	std::shared_ptr<Texture> blackOverlay;

	std::shared_ptr<PixelShader> PS_buttonUI;
	std::shared_ptr<PixelShader> PS_defaultUI;
	std::shared_ptr<PixelShader> PS_primitives;
	std::shared_ptr<PixelShader> PS_pbrModel;
	std::shared_ptr<PixelShader> PS_interactiveStaticPBR;
	std::shared_ptr<PixelShader> PS_interactiveStaticNormal;
	std::shared_ptr<PixelShader> PS_Shadow;
	std::shared_ptr<PixelShader> PS_WriteDepth;

	std::shared_ptr<VertexShader> VS_defaultUI;
	std::shared_ptr<VertexShader> VS_pbrModel;
	std::shared_ptr<VertexShader> VS_primitives;
	std::shared_ptr<VertexShader> VS_spriteShadow;

	std::shared_ptr<Player> mPlayer;

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
	void SetSwitchScene(bool isSwitchScene);
	void SetSwitchSceneWithFade(bool isSwitchScene);

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

	void InitPlayer(){};
};

