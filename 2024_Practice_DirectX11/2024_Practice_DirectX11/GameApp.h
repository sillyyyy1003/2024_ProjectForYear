#pragma once
#include <FastNoise/FastNoiseLite.h>
#include "d3dApp.h"
#include "SceneManager.h"


class GameApp : public D3DApp
{
private:

    //========= ゲーム用変数 ============
    static FastNoiseLite GameNoise;//One Instance is enough
    int mCurrentRenderIndex;//Using Double Buffer
    //========= ゲーム用変数 ============
private:
    GameApp() {};
    ~GameApp();

public:

    static GameApp* Get() { static GameApp instance; return &instance; }

    template<class T>
    static std::shared_ptr<T> GetComponent(const char* objName) { return SceneManager::Get()->GetObj<T>(objName); };

    static CameraBase* GetCurrentCamera() { return SceneManager::Get()->GetCurrentCamera(); };
    static FastNoiseLite& GetNoise() { return GameNoise; };

    static int GetCurrentRenderIndex() { return Get()->mCurrentRenderIndex; };

    /// @brief Timerを取得する
	Timer& GetTimer() { return Get()->mTimer; };

    /// @brief 
    /// @return 
    bool Init() override;

    /// @brief 
    void OnResize() override;

    /// @brief
    /// @param deltaTime tick
    void UpdateScene(float deltaTime) override;

    /// @brief 
    void DrawScene() override;

    void UnInit() override;

   

private:

    bool InitResource();

};
