#pragma once
#include "SceneBase.h"

/// @brief 紙の巡り効果や物理挙動を行う
class Paper:public Component
{
	struct VertexPhysics
	{
		bool onLock;		//固定部分
		DirectX::XMFLOAT3 Velocity;		//速度
		DirectX::XMFLOAT3 acceleration;	//加速度
		DirectX::XMFLOAT3 resultant;	//合力

	};


};

