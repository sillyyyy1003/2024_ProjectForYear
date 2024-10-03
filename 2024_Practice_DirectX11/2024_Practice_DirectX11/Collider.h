#pragma once
#include "D3DUtil.h"
#include <DirectXCollision.h>
#include "SceneBase.h"


class Collider :public Component
{
protected:

	enum ColliderKind
	{
		DEFAULT_3D,
		CAPSULE_3D,
		BOX_3D,
		SPHERE_3D,
		CYLINDER_3D,
	};
	ColliderKind mKind = DEFAULT_3D;

public:

	Collider(ColliderKind kind);
	virtual ~Collider();//virtual method

	/// @brief �R���C�_�[�ʒu�Ɖ�]�̍X�V
	/// @param pos �ʒu
	/// @param rot ��]
	///	@param scale �g��k���̔䗦
	virtual void Transform(DirectX::XMFLOAT3 pos,DirectX::XMVECTOR rot,DirectX::XMFLOAT3 scale) = 0;

	/// @brief �R���C�_�[�傫��
	/// @param size �傫��
	virtual void UpdateSize(DirectX::XMFLOAT3 size){};
	virtual void UpdateSize(float size) {};

	virtual DirectX::XMFLOAT4 GetOrientation() { return { 0,0,0,1 }; };
	virtual const DirectX::XMFLOAT3& GetCenter() = 0;
	virtual const float GetRadius() { return 0; };
	virtual const DirectX::XMFLOAT3& GetScale() { return { 0,0,0 }; };

	virtual bool Interacts(DirectX::XMVECTOR start, DirectX::XMVECTOR des, float& distance) = 0;

};

/// @brief �{�b�N�X�R���C�_�[
class BoxCollider :public Collider
{
private:

	DirectX::BoundingOrientedBox mCollider;
	//std::unique_ptr<btBoxShape> mShape;	//�����蔻��̌`����Ǘ�����
	//std::unique_ptr<btCollisionObject> mBoxCollider;
	//btTransform mTransform;	//�ړ��A��]�A�g��k���Ȃǂ��Ǘ�����

public:

	BoxCollider();
	~BoxCollider() override = default;

	/// @brief �R���C�_�[�ϊ�
	/// @param pos �ړ���̒��S�ʒu
	/// @param rot ��]�p�x(�l����)
	///	@param scale �g��k���̔䗦
	void Transform(DirectX::XMFLOAT3 pos, DirectX::XMVECTOR rot, DirectX::XMFLOAT3 scale) override;

	/// @brief �傫����ݒ肷��
	/// @param size �ݒ肵�����T�C�Y
	void UpdateSize(DirectX::XMFLOAT3 size) override;

	/// @brief ���C�Ƃ̌������Ă邩�ǂ���
	/// @param start �����̌��_
	/// @param des �����̕���
	/// @param distance �ː��ƕ\�ʂ̈�ԋ߂�����
	/// @return ���C�Ƃ̌������Ă邩�ǂ���
	bool Interacts(DirectX::XMVECTOR start, DirectX::XMVECTOR des, float& distance) override;

	DirectX::XMFLOAT4 GetOrientation() override;
	const DirectX::XMFLOAT3& GetCenter() override;
	const DirectX::XMFLOAT3& GetScale() override;

};

class SphereCollider :public Collider
{
private:

	DirectX::BoundingSphere mCollider;

public:

	SphereCollider();
	~SphereCollider() override = default;

	/// @brief �R���C�_�[�ϊ�
	/// @param pos �ړ���̒��S�ʒu
	/// @param rot ��]�p�x(�l����)
	///	@param scale �g��k���̔䗦
	void Transform(DirectX::XMFLOAT3 pos, DirectX::XMVECTOR rot, DirectX::XMFLOAT3 scale) override;

	/// @brief ���C�Ƃ̌������Ă邩�ǂ���
	/// @param start �����̌��_
	/// @param des �����̕���
	/// @param distance �ː��ƕ\�ʂ̈�ԋ߂�����
	/// @return ���C�Ƃ̌������Ă邩�ǂ���
	bool Interacts(DirectX::XMVECTOR start, DirectX::XMVECTOR des, float& distance) override;

	/// @brief ���S�_���擾
	/// @return �R���C�_�[�̒��S�_
	const DirectX::XMFLOAT3& GetCenter() override { return mCollider.Center; };

	/// @brief ���a���擾
	/// @return ���a
	const float GetRadius() override { return mCollider.Radius; };

	void UpdateSize(float radius)override;
};