#include "Ingredient.h"

#include "GampApp.h"

enum IngredientState
{
	STATE_NONE,
	STATE_USE,
};

Ingredient::Ingredient()
{
}

void Ingredient::Init(PrimitiveKind _kind, const char* _fileName)
{
	//todo Change Shader loading
	Object::Init(_kind, _fileName);
	
}

void Ingredient::SetPrice(float _price)
{
	mFreshness = _price;
}


void Ingredient::SetName(const char* _ingredientName)
{
	mIngredientName = _ingredientName;
	mObjectName = mIngredientName;
}

void Ingredient::Update(float dt)
{

	PreUpdate(dt);

	GameUpdate(dt);

	LateUpdate(dt);

}

void Ingredient::Draw()
{
	if(mIngredientState!=STATE_USE)
		Object::Draw();
}

void Ingredient::UpdatePrice()
{
	//todo: need to fix the transform problem
	if (mFreshness > 0)
	{
		mFreshness = 100.f * (1.f - GameApp::Get()->GetTimer().GetGameHourPassed() / mShelfLife);

	}
	else
	{
		mFreshness = 0;
	}

	mCurrPrice = mBasicPrice * (mFreshness / 100);
}

void Ingredient::PreUpdate(float dt)
{
	UpdatePrice();
	Object::PreUpdate(dt);
}

void Ingredient::GameUpdate(float dt)
{
	Object::GameUpdate(dt);
}

void Ingredient::LateUpdate(float dt)
{
	Object::LateUpdate(dt);
}

std::string Ingredient::GetIngredientInfo()
{
	std::stringstream infoStr;

	infoStr << "Name: " << mIngredientName << "\n"
		<< "Freshness: " << mFreshness << "\n"
		<< "Price: " << mCurrPrice << "\n";



	return infoStr.str();
}


