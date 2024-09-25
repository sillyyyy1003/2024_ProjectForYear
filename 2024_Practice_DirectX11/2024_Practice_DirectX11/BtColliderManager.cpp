#include "BtColliderManager.h"

BtColliderManager::BtColliderManager()
{
}

BtColliderManager& BtColliderManager::Get()
{
	static BtColliderManager instance;
	return instance;
}

bool BtColliderManager::Init()
{
	mBroadPhase=std::make_unique<btBroadphaseInterface>();
	mCollisionConfiguration = std::make_unique<btCollisionConfiguration>();
	mDispatcher = std::make_unique<btCollisionDispatcher>(mCollisionConfiguration.get());
	mCollisionWorld = std::make_unique<btCollisionWorld>(mDispatcher.get(),mBroadPhase.get(),mCollisionConfiguration.get());

	return true;
}

