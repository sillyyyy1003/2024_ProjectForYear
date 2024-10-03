#pragma once
#include <map>
#include <memory>
#include <string>
#include "SceneBase.h"

class Player
{
private:

	float mGold = 0;	//�v�����[������
	int mWitchLevel = 1;//�M���h���x��

	//the components will all be copied from SceneBase::mObjects;
	using Ingredients = std::map<std::string, Component*>;
	static Ingredients mIngredients;	//����Ingredients

	//todo: �S������̗e��ň���

public:
	Player();
	~Player() = default;

	void AddIngredient(Ingredients* _component);

	void Update(float dt);

	void DrawObject();


};

