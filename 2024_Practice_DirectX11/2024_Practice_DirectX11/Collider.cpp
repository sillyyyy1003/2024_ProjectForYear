#include "Collider.h"

#include <BulletCollision/CollisionShapes/btCylinderShape.h>

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

	mBoxCollider = std::make_unique<btBoxShape>(btVector3(0.5f, 0.5f, 0.5f));

	mTransform = std::make_unique<btTransform>();
	mTransform->setIdentity();
	mTransform->setOrigin(btVector3(0.f, 0.f, 0.f));

}

void BoxCollider::Transform(DirectX::XMFLOAT3 pos, DirectX::XMVECTOR rot, DirectX::XMFLOAT3 scale)
{
	//mCollider.Center = pos;
	//XMStoreFloat4(&mCollider.Orientation, rot);
	//Vector3 extents = mCollider.Extents;

	//extents *= scale;
	//mCollider.Extents = extents;
	XMFLOAT4 quaternion;
	XMStoreFloat4(&quaternion, rot);
	mTransform->setOrigin(btVector3(pos.x, pos.y, pos.z));
	mTransform->setRotation(btQuaternion(quaternion.x, quaternion.y, quaternion.z, quaternion.w));
	btVector3 newScale = btVector3(scale.x, scale.y, scale.z);
	mBoxCollider->setLocalScaling(newScale);

}

void BoxCollider::UpdateSize(DirectX::XMFLOAT3 size)
{
	mCollider.Extents = size;

}

DirectX::XMFLOAT4 BoxCollider::GetOrientation()
{
	return Collider::GetOrientation();
}

DirectX::XMFLOAT3 BoxCollider::GetCenter()
{
}

void BoxCollider::RayTest(DirectX::XMVECTOR start, DirectX::XMVECTOR des,
	btCollisionWorld::RayResultCallback& resultCallback)
{
	Vector3 startPos,desPos;
	XMStoreFloat3(&startPos, start);
	XMStoreFloat3(&desPos, des);
	desPos *= 100;
	btVector3 s = btVector3(startPos.x, startPos.y, startPos.z);
	btVector3 d = btVector3(desPos.x, desPos.y, desPos.z);

	btCollisionWorld::ClosestRayResultCallback rayCallback(s, d);



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

CylinderCollider::CylinderCollider() :Collider(CYLINDER_3D)
{
	mCollider = std::make_unique<btCylinderShape>(btVector3(0.5f, 0.5f, 0.5f));
}



