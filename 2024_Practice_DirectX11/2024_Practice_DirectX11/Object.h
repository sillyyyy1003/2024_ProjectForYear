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
	virtual ~Object();

	/// @brief �I�u�W�F�N�g���Ԃ̏�����
	/// @param kind �I�u�W�F�N�g�̌`��
	/// @param fileName �e�N�X�`���t�@�C���p�X
	void Init(PrimitiveKind kind, const char* fileName);
	void InitPosition(DirectX::XMFLOAT3& pos);

	virtual void Update(float dt);
	virtual void Draw();

	DirectX::XMFLOAT3 GetPosition() const { return mModel->GetPosition(); };
	DirectX::XMFLOAT3 GetScale() const { return mModel->GetScale(); };
	DirectX::XMFLOAT4 GetQuaternion() const { return mModel->GetQuaternion(); };
	DirectX::XMFLOAT3 GetRotation()const { return mModel->GetRotation(); }

	std::string GetFilePath() const { return mModel->GetFilePath(); };
	Material GetMaterial() { return mModel->GetMaterial(); };

	void SetScale(const DirectX::XMFLOAT3& scale) { mModel->SetScale(scale); };
	void SetScale(const DirectX::XMFLOAT2& scale) {};
	void SetScale(const float* scale) { mModel->mTransform.SetScale(scale); };

	void SetPosition(const DirectX::XMFLOAT3 position) { mModel->mTransform.SetPosition(position); };
	void SetPosition(float x, float y, float z) { mModel->mTransform.SetPosition(x, y, z); };
	void SetPosition(const float* position) { mModel->mTransform.SetPosition(position); };

	void SetMaterial(const Material& mat);

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

