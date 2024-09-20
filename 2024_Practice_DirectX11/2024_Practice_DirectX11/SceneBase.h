#pragma once
#include "D3DUtil.h"
#include <list>
#include <map>
#include <memory>
#include <string>
#include <nlohmann/json.hpp>

#include "DebugLog.h"
using json = nlohmann::json;
using namespace DirectX::SimpleMath;

/// @brief �Q�[���V�[���Ŏg�����ׂẴI�u�W�F�N�g�̊��ɂȂ�
class Component
{
public:
	virtual ~Component() {}
};

/// @brief Scene Load Object
/// @tparam T ����I�u�W�F�N�g�Ή�
template<class T>
class SceneObject : public Component
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
	using Objects = std::map<std::string, std::shared_ptr<Component>>;
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

	/// @brief json�t�@�C���ɃI�u�W�F�N�g�f�[�^�ۑ�
	/// @tparam T �I�u�W�F�N�g�^
	/// @param obj 
	/// @return 
	template<class T>
	json SaveData(T* obj);


	/// @brief �T�u�V�[���̒ǉ�
	/// @tparam T �T�u�V�[���̌^
	///	@return ���������T�u�V�[��
	template<class T> T* AddSubScene();

	/// @brief �V�[���p��
	void RemoveSubScene() const;

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
json SceneBase::SaveData(T* obj)
{
	json data;
	data["Position"] = { obj->GetPosition().x,obj->GetPosition().y,obj->GetPosition().z };
	data["Scale"] = { obj->GetScale().x,obj->GetScale().y,obj->GetScale().z };
	data["Rotation"] = { obj->GetRotation().x,obj->GetRotation().y,obj->GetRotation().z };
	data["Filepath"] = obj->GetFilePath();

	//Set Material
	data["Material"]["Ambient"] = {obj->GetMaterial().ambient.x,obj->GetMaterial().ambient.y, obj->GetMaterial().ambient.z, obj->GetMaterial().ambient.w};
	data["Material"]["Diffuse"] = { obj->GetMaterial().diffuse.x, obj->GetMaterial().diffuse.y, obj->GetMaterial().diffuse.z, obj->GetMaterial().diffuse.w};
	data["Material"]["Specular"] = { obj->GetMaterial().specular.x,obj->GetMaterial().specular.y,obj->GetMaterial().specular.z,obj->GetMaterial().specular.w };
	data["Material"]["Emission"] = { obj->GetMaterial().emission.x, obj->GetMaterial().emission.y, obj->GetMaterial().emission.z, obj->GetMaterial().emission.w};
	
	return data;
}


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
	mObjects.insert(std::pair<std::string, std::shared_ptr<Component>>(_name, std::make_shared<SceneObject<T>>(ptr)));
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


