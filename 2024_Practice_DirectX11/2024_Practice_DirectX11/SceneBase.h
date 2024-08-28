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
/// @tparam T 多種オブジェクト対応
template<class T>
class SceneObject : public SceneObjectBase
{
public:
	SceneObject(std::shared_ptr<T> ptr) : mObj(ptr) {}
	virtual ~SceneObject() {}
	std::shared_ptr<T> mObj;
};

/// @brief シーン基本クラス(Abstract factory pattern?)
class SceneBase
{
private:
	using Objects = std::map<std::string, std::shared_ptr<SceneObjectBase>>;
	using Items = std::list<std::string>;

	static Objects mObjects;//シーン内すべてのオブジェクト

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


	/// @brief サブシーンの追加
	/// @tparam T サブシーンの型
	///	@return 生成したサブシーン
	template<class T> T* AddSubScene();

	/// @brief シーン廃棄
	void RemoveSubScene() const;

	/// @brief オブジェクトの生成
	/// @tparam T オブジェクトの型
	/// @param _name オブジェクトの名称
	/// @return 生成したオブジェクト
	//template<class T> T* CreateObj(const char* _name);

	template<class T> T* CreateObj(const char* _name);

	/// @brief オブジェクト破棄
	void DestroyObj(const char* name);

	/// @brief オブジェクトの取得
	/// @tparam T オブジェクトの型
	/// @param _name オブジェクトの名称
	/// @return 取得したオブジェクト
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
	// デバッグ中のみ、名称ダブりがないかチェック
	Objects::iterator it = mObjects.find(_name);
	if (it != mObjects.end()) {
		static char buf[256];
		sprintf_s(buf, sizeof(buf), "Failed to create object. %s", _name);
		DebugLog::LogError(buf);
		return nullptr;
	}
#endif // _DEBUG

	// オブジェクト生成
	std::shared_ptr<T> ptr = std::make_shared<T>();
	mObjects.insert(std::pair<std::string, std::shared_ptr<SceneObjectBase>>(_name, std::make_shared<SceneObject<T>>(ptr)));
	mItems.push_back(_name);
	return ptr.get();
}

template <class T>
T* SceneBase::GetObj(const char* _name)
{
	// オブジェクトの探索
	Objects::iterator it = mObjects.find(_name);
	if (it == mObjects.end()) return nullptr;

	// 型変換
	std::shared_ptr<SceneObject<T>> ptr = std::reinterpret_pointer_cast<SceneObject<T>>(it->second);
	return ptr->mObj.get();
}


