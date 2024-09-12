#pragma once
#include "Collider.h"
#include "Primitive.h"


/// @brief 基本オブジェクト・当たり判定付き
///	For Test
class Object
{
private:

	POINT mOldPos = {};

	int mState = 0;//オブジェクトの状態を司る
	bool isStateChange = false;//状態変更判定

public:
	
	std::unique_ptr<Primitive> 	mModel = nullptr;		//描画
	std::unique_ptr<Collider> 	mCollider = nullptr;	//当たり判定

public:
	Object();
	~Object();

	/// @brief オブジェクト実態の初期化
	/// @param kind オブジェクトの形状
	/// @param fileName テクスチャファイルパス
	void Init(PrimitiveKind kind, const char* fileName);

	void Update(float dt);
	void Draw();


private:

	/// @brief 入力・物理演算などの処理を行うフラッグの更新
	/// @param dt Delta Time
	void PreUpdate(float dt);

	/// @brief 入力などに応じて、ゲームロジックの実現
	/// @param dt Delta Time
	void GameUpdate(float dt);

	/// @brief 
	/// @param dt Delta Time
	void LateUpdate(float dt);

	/// @brief Drag状態処理
	/// @param dt Delta Time
	void OnStateDrag(float dt);

	/// @brief Selected状態処理
	/// @param dt Delta Time
	void OnStateSelected(float dt);

};

