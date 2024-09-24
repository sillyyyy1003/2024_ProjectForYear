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

json SceneBase::LoadSceneData(const char* fileName)
{
	std::filesystem::path filePath = fileName;
	if (!std::filesystem::exists(filePath))
	{
		DebugLog::Log("scene_title.json not found.");
		return nullptr;
	}

	std::ifstream file(filePath);
	if (!file.is_open())
	{
		DebugLog::Log("Failed to open scene_title.json for reading.");
		return nullptr;
	}

	json sceneData;
	file >> sceneData;
	file.close();

	return sceneData;
}

void SceneBase::RemoveSubScene() const
{
	// 削除するサブシーンが存在するか
	if (!mpSubScene) return;

	// 階層内のサブシーンを優先して削除
	mpSubScene->RemoveSubScene();

	// 直下のサブシーンを削除
	mpSubScene->UnInit();

	// 释放内存并将指针置空
	delete mpSubScene;
}

void SceneBase::DestroyObj(const char* name)
{
	mObjects.erase(name);
	mItems.remove(name);
}
