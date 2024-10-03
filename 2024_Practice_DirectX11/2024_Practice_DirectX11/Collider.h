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

	/// @brief コライダー位置と回転の更新
	/// @param pos 位置
	/// @param rot 回転
	///	@param scale 拡大縮小の比率
	virtual void Transform(DirectX::XMFLOAT3 pos,DirectX::XMVECTOR rot,DirectX::XMFLOAT3 scale) = 0;

	/// @brief コライダー大きさ
	/// @param size 大きさ
	virtual void UpdateSize(DirectX::XMFLOAT3 size){};
	virtual void UpdateSize(float size) {};

	virtual DirectX::XMFLOAT4 GetOrientation() { return { 0,0,0,1 }; };
	virtual const DirectX::XMFLOAT3& GetCenter() = 0;
	virtual const float GetRadius() { return 0; };
	virtual const DirectX::XMFLOAT3& GetScale() { return { 0,0,0 }; };

	virtual bool Interacts(DirectX::XMVECTOR start, DirectX::XMVECTOR des, float& distance) = 0;

};

/// @brief ボックスコライダー
class BoxCollider :public Collider
{
private:

	DirectX::BoundingOrientedBox mCollider;
	//std::unique_ptr<btBoxShape> mShape;	//あたり判定の形状を管理する
	//std::unique_ptr<btCollisionObject> mBoxCollider;
	//btTransform mTransform;	//移動、回転、拡大縮小などを管理する

public:

	BoxCollider();
	~BoxCollider() override = default;

	/// @brief コライダー変換
	/// @param pos 移動後の中心位置
	/// @param rot 回転角度(四元数)
	///	@param scale 拡大縮小の比率
	void Transform(DirectX::XMFLOAT3 pos, DirectX::XMVECTOR rot, DirectX::XMFLOAT3 scale) override;

	/// @brief 大きさを設定する
	/// @param size 設定したいサイズ
	void UpdateSize(DirectX::XMFLOAT3 size) override;

	/// @brief レイとの交差してるかどうか
	/// @param start 光線の原点
	/// @param des 光線の方向
	/// @param distance 射線と表面の一番近い距離
	/// @return レイとの交差してるかどうか
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

	/// @brief コライダー変換
	/// @param pos 移動後の中心位置
	/// @param rot 回転角度(四元数)
	///	@param scale 拡大縮小の比率
	void Transform(DirectX::XMFLOAT3 pos, DirectX::XMVECTOR rot, DirectX::XMFLOAT3 scale) override;

	/// @brief レイとの交差してるかどうか
	/// @param start 光線の原点
	/// @param des 光線の方向
	/// @param distance 射線と表面の一番近い距離
	/// @return レイとの交差してるかどうか
	bool Interacts(DirectX::XMVECTOR start, DirectX::XMVECTOR des, float& distance) override;

	/// @brief 中心点を取得
	/// @return コライダーの中心点
	const DirectX::XMFLOAT3& GetCenter() override { return mCollider.Center; };

	/// @brief 半径を取得
	/// @return 半径
	const float GetRadius() override { return mCollider.Radius; };

	void UpdateSize(float radius)override;
};