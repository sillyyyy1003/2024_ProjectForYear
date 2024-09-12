#include "Collider.h"
#include "FirstPersonCamera.h"
#include "GampApp.h"
using namespace DirectX::SimpleMath;
using namespace DirectX;

Collider::Collider(ColliderKind kind) :mKind(kind)
{
}

Collider::~Collider()
{
}


BoxCollider::BoxCollider() :Collider(BOX_3D)
{
	mCollider.Center = { 0,0,0 };
	mCollider.Extents = { 0.5,0.5,0.5 };//Extent is half width
	mCollider.Orientation = { 0,0,0,1 };

}

void BoxCollider::Transform(DirectX::XMFLOAT3 pos, DirectX::XMVECTOR rot, DirectX::XMFLOAT3 scale)
{
	//移動量を求める
	/*
	XMFLOAT3 moveVal = pos- mCollider.Center;
	XMVECTOR translation = XMLoadFloat3(&moveVal);
	//mCollider.Center = pos;

	//回転量を求める
	//Normalize vec
	XMVECTOR rotation = XMQuaternionNormalize(rot);

	//オリエンテーションの値をずっと単位ベクトルにする
	XMVECTOR oriVec = XMQuaternionNormalize(XMLoadFloat4(&mCollider.Orientation));
	
	//コライダーの拡大縮小を固定
	mCollider.Transform(mCollider, 1.f, rotation, translation);*/
	mCollider.Center = pos;
	XMStoreFloat4(&mCollider.Orientation, rot);
	Vector3 extents = mCollider.Extents;

	extents *= scale;
	mCollider.Extents = extents;
}

void BoxCollider::UpdateSize(DirectX::XMFLOAT3 size)
{
	mCollider.Extents = size;
}

bool BoxCollider::Interacts(DirectX::XMVECTOR start, DirectX::XMVECTOR des, float& distance)
{
	return mCollider.Intersects(start, des, distance);
}

SphereCollider::SphereCollider():Collider(SPHERE_3D)
{
	mCollider.Center = { 0,0,0 };
	mCollider.Radius = 0.5f;
}

void SphereCollider::Transform(DirectX::XMFLOAT3 pos, DirectX::XMVECTOR rot, DirectX::XMFLOAT3 scale)
{
	mCollider.Center = pos;
	mCollider.Radius *= scale.x;
}


bool SphereCollider::Interacts(DirectX::XMVECTOR start, DirectX::XMVECTOR des, float& distance)
{
	return mCollider.Intersects(start, des, distance);
	
}

void SphereCollider::UpdateSize(float radius)
{
	mCollider.Radius = radius;
}


