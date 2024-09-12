#pragma once
#include "Collider.h"
#include "Primitive.h"


/// @brief ��{�I�u�W�F�N�g�E�����蔻��t��
///	For Test
class Object
{
private:

	POINT mOldPos = {};

	int mState = 0;//�I�u�W�F�N�g�̏�Ԃ��i��
	bool isStateChange = false;//��ԕύX����

public:
	
	std::unique_ptr<Primitive> 	mModel = nullptr;		//�`��
	std::unique_ptr<Collider> 	mCollider = nullptr;	//�����蔻��

public:
	Object();
	~Object();

	/// @brief �I�u�W�F�N�g���Ԃ̏�����
	/// @param kind �I�u�W�F�N�g�̌`��
	/// @param fileName �e�N�X�`���t�@�C���p�X
	void Init(PrimitiveKind kind, const char* fileName);

	void Update(float dt);
	void Draw();


private:

	/// @brief ���́E�������Z�Ȃǂ̏������s���t���b�O�̍X�V
	/// @param dt Delta Time
	void PreUpdate(float dt);

	/// @brief ���͂Ȃǂɉ����āA�Q�[�����W�b�N�̎���
	/// @param dt Delta Time
	void GameUpdate(float dt);

	/// @brief 
	/// @param dt Delta Time
	void LateUpdate(float dt);

	/// @brief Drag��ԏ���
	/// @param dt Delta Time
	void OnStateDrag(float dt);

	/// @brief Selected��ԏ���
	/// @param dt Delta Time
	void OnStateSelected(float dt);

};

