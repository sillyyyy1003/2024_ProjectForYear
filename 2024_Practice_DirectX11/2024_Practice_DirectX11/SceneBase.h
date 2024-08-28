#pragma once
#include <list>
#include <map>
#include <memory>
#include <memory>
#include <string>
#include "DebugLog.h"

/// @brief Game Obj Base
class SceneObjectBase
{
public:
	virtual ~SceneObjectBase() {}
};

/// @brief Scene Load Object
/// @tparam T ����I�u�W�F�N�g�Ή�
template<class T>
class SceneObject : public SceneObjectBase
{
public:
	SceneObject(std::shared_ptr<T> ptr) : mObj(ptr) {}
	virtual ~SceneObject() {}
	std::shared_ptr<T> mObj;
};

/// @brief �V�[����{�N���X(Abstract factory pattern?)
class SceneBase
{
private:
	using Objects = std::map<std::string, std::shared_ptr<SceneObjectBase>>;
	using Items = std::list<std::string>;

	static Objects mObjects;//�V�[�������ׂẴI�u�W�F�N�g

protected:
	/*
	 *How to manage the scene (CN)
	 *https://www.notion.so/ffff308cb50d80749370deb76e71aad0?pvs=4
	 */
	SceneBase* mpParentScene = nullptr; //Parent Scene
	SceneBase* mpSubScene = nullptr; //Sub Scene
	Items mItems;
public:
	SceneBase();
	virtual ~SceneBase();

	void _update(float dt);
	void _draw();


	/// @brief �T�u�V�[���̒ǉ�
	/// @tparam T �T�u�V�[���̌^
	///	@return ���������T�u�V�[��
	template<class T> T* AddSubScene();

	/// @brief �V�[���p��
	void RemoveSubScene() const;

	/// @brief �I�u�W�F�N�g�̐���
	/// @tparam T �I�u�W�F�N�g�̌^
	/// @param _name �I�u�W�F�N�g�̖���
	/// @return ���������I�u�W�F�N�g
	//template<class T> T* CreateObj(const char* _name);

	template<class T> T* CreateObj(const char* _name);

	/// @brief �I�u�W�F�N�g�j��
	void DestroyObj(const char* name);

	/// @brief �I�u�W�F�N�g�̎擾
	/// @tparam T �I�u�W�F�N�g�̌^
	/// @param _name �I�u�W�F�N�g�̖���
	/// @return �擾�����I�u�W�F�N�g
	template<class T> T* GetObj(const char* _name);


	// virtual function
	virtual void Init() = 0;
	virtual void UnInit() = 0;
	virtual void Update(float tick) = 0;
	virtual void Draw() = 0;
};

template <class T>
T* SceneBase::AddSubScene()
{
	RemoveSubScene();
	T* pScene = new T;
	mpSubScene = pScene;
	pScene->mpParentScene = this;
	pScene->Init();
	return pScene;
}


template <class T>
T* SceneBase::CreateObj(const char* _name)
{
#ifdef _DEBUG
	// �f�o�b�O���̂݁A���̃_�u�肪�Ȃ����`�F�b�N
	Objects::iterator it = mObjects.find(_name);
	if (it != mObjects.end()) {
		static char buf[256];
		sprintf_s(buf, sizeof(buf), "Failed to create object. %s", _name);
		DebugLog::LogError(buf);
		return nullptr;
	}
#endif // _DEBUG

	// �I�u�W�F�N�g����
	std::shared_ptr<T> ptr = std::make_shared<T>();
	mObjects.insert(std::pair<std::string, std::shared_ptr<SceneObjectBase>>(_name, std::make_shared<SceneObject<T>>(ptr)));
	mItems.push_back(_name);
	return ptr.get();
}

template <class T>
T* SceneBase::GetObj(const char* _name)
{
	// �I�u�W�F�N�g�̒T��
	Objects::iterator it = mObjects.find(_name);
	if (it == mObjects.end()) return nullptr;

	// �^�ϊ�
	std::shared_ptr<SceneObject<T>> ptr = std::reinterpret_pointer_cast<SceneObject<T>>(it->second);
	return ptr->mObj.get();
}


