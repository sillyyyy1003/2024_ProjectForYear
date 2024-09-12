#pragma once
#include <memory>
#include "d3dApp.h"
#include "Geometry.h"
#include "SceneManager.h"
#include "UI2D.h"

class GameApp : public D3DApp
{
private:


    //========= ゲーム用オブジェクト ============
    std::unique_ptr<UI2D> ui = nullptr;
    //========= ゲーム用オブジェクト ============





	//========= ゲーム用変数 ============

    //========= ゲーム用変数 ============

public:

 
   

    
private:
    GameApp(){};
    ~GameApp();

public:

    static GameApp* Get(){ static GameApp instance; return &instance; }

	template<class T>
    static T* GetComponent(const char* objName) { return SceneManager::Get()->GetObj<T>(objName); };

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
