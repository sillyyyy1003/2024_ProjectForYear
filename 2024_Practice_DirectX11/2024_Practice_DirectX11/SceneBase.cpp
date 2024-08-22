#include "SceneBase.h"

SceneBase::Objects SceneBase::mObjects;

SceneBase::SceneBase()
{
	//~
}

SceneBase::~SceneBase()
{
	// �T�u�V�[�����폜
	RemoveSubScene();

	// �폜
	Items::iterator it = mItems.begin();

	while (it != mItems.end())
	{
		mObjects.erase(*it);
		++it;
	}
	mItems.clear();

	// �e�̎Q�Ƃ��폜
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
	// �폜����T�u�V�[�������݂��邩
	if (!mpSubScene) return;

	// �K�w���̃T�u�V�[����D�悵�č폜
	mpSubScene->RemoveSubScene();

	// �����̃T�u�V�[�����폜
	mpSubScene->UnInit();
}

void SceneBase::DestroyObj(const char* name)
{
	mObjects.erase(name);
	mItems.remove(name);
}
