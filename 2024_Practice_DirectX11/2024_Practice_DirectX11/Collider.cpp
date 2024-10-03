#include "Collider.h"
#include "Cube.h"
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
	mCollider.Center = pos;
	XMStoreFloat4(&mCollider.Orientation, rot);
	Vector3 extents = scale / 2;
	mCollider.Extents = extents;
}

void BoxCollider::UpdateSize(DirectX::XMFLOAT3 size)
{
	mCollider.Extents = size;

}

DirectX::XMFLOAT4 BoxCollider::GetOrientation()
{
	return Collider::GetOrientation();
}

const DirectX::XMFLOAT3& BoxCollider::GetCenter()
{
	//btVector3 center = mTransform.getOrigin();
	//Vector3 pos = Vector3(center.x(), center.y(), center.z());
	//return pos;
	//return{ 0,0,0 };
	return mCollider.Center;
}

const DirectX::XMFLOAT3& BoxCollider::GetScale()
{
	return mCollider.Extents;
}

bool BoxCollider::Interacts(DirectX::XMVECTOR start, DirectX::XMVECTOR des, float& distance)
{
	return mCollider.Intersects(start, des, distance);
}


SphereCollider::SphereCollider() :Collider(SPHERE_3D)
{
	mCollider.Center = { 0,0,0 };
	mCollider.Radius = 0.5f;
}

void SphereCollider::Transform(DirectX::XMFLOAT3 pos, DirectX::XMVECTOR rot, DirectX::XMFLOAT3 scale)
{
	mCollider.Center = pos;
	mCollider.Radius = scale.x / 2.0f;
}


bool SphereCollider::Interacts(DirectX::XMVECTOR start, DirectX::XMVECTOR des, float& distance)
{
	return mCollider.Intersects(start, des, distance);

}

void SphereCollider::UpdateSize(float radius)
{
	mCollider.Radius = radius;
}

