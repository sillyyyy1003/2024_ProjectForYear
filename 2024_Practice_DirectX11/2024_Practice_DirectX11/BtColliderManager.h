#pragma once
#include <memory>
#include "btBulletCollisionCommon.h"

class BtColliderManager
{
	std::unique_ptr<btBroadphaseInterface> mBroadPhase;
	std::unique_ptr<btCollisionConfiguration> mCollisionConfiguration;
	std::unique_ptr<btCollisionDispatcher> mDispatcher;
	std::unique_ptr<btCollisionWorld> mCollisionWorld;


	BtColliderManager();
	~BtColliderManager() = default;

private:

	/// @brief �V���O���g���C���X�^���X�擾
	static BtColliderManager& Get();

public:
	bool Init();


};

