#pragma once
#include <map>
#include <memory>
#include <string>
#include "SceneBase.h"

class Player
{
private:

	float mGold = 0;	//プレヤーが持つ金
	int mWitchLevel = 1;//ギルドレベル

	//the components will all be copied from SceneBase::mObjects;
	using Ingredients = std::map<std::string, Component*>;
	static Ingredients mIngredients;	//持つIngredients

	//todo: 全部を一個の容器で扱う

public:
	Player();
	~Player() = default;

	void AddIngredient(Ingredients* _component);

	void Update(float dt);

	void DrawObject();


};

