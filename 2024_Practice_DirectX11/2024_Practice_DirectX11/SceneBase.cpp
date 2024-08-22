#include "SceneBase.h"

SceneBase::Objects SceneBase::mObjects;

SceneBase::SceneBase()
{
	//~
}

SceneBase::~SceneBase()
{
	// サブシーンを削除
	RemoveSubScene();

	// 削除
	Items::iterator it = mItems.begin();

	while (it != mItems.end())
	{
		mObjects.erase(*it);
		++it;
	}
	mItems.clear();

	// 親の参照を削除
	if (mpParentScene)
		mpParentScene->mpSubScene = nullptr;
}

void SceneBase::_update(float dt)
{

	if (mpSubScene)
		mpSubScene->_update(dt);
	Update(dt);
}

void SceneBase::_draw()
{
	if (mpSubScene)
		mpSubScene->_draw();
	Draw();
}

void SceneBase::RemoveSubScene() const
{
	// 削除するサブシーンが存在するか
	if (!mpSubScene) return;

	// 階層内のサブシーンを優先して削除
	mpSubScene->RemoveSubScene();

	// 直下のサブシーンを削除
	mpSubScene->UnInit();
}

void SceneBase::DestroyObj(const char* name)
{
	mObjects.erase(name);
	mItems.remove(name);
}
