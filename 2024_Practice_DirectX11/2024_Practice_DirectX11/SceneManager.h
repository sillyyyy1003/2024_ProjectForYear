#pragma once
#include <unordered_map>

#include "SceneBase.h"

/// @brief すべてのシーンの管理
class SceneManager :public SceneBase
{
private:

	int mIndex = 0;//Scene Index
	std::unordered_map<std::string, int> mSceneMap;

private:
	SceneManager(){};
	~SceneManager() = default;


public:
	static SceneManager* Get() { static SceneManager instance; return &instance; };

	/// @brief 共通オブジェクトの読み込み・作成
	void Init();

	/// @brief データ保存・リソースリリース
	void UnInit();

	/// @brief 
	/// @param dt deltaTime
	void Update(float dt);

	/// @brief 描画
	void Draw();

	void SetScene(std::string sceneName);

private:
	/// @brief シーンの切り替え
	void ChangeScene();


};

