#pragma once
#include "D3DUtil.h"
#include "SceneBase.h"

//todo:内容不完整，做完blendState和水的效果之后再回来做RigidBody
class RigidBody :public Component
{
	DirectX::XMFLOAT3 mPos={};
    DirectX::XMFLOAT3 velocity = {};    	// 速度
    DirectX::XMFLOAT3 acceleration = {};  	// 加速度
    DirectX::XMFLOAT3 force = {};      		// すべての力
    DirectX::XMFLOAT3 gravity ={};    		// 重力
    float mass = 0;          				// 重さ
    float drag = 0;                         // 抵抗

    bool isPhysics = false;                 // 物理シミレの切り替え

public:
    RigidBody();
    ~RigidBody();

    void Init();


    void SetMass(float _mass) { mass = _mass; };
    float GetMass() const { return mass; };

    void SetDrag(float _drag) { drag = _drag; };
    float GetDrag() const { return drag; };

    void SetPos(const DirectX::XMFLOAT3& _pos) { mPos = _pos; };
    void SetPos(float x, float y, float z) { mPos = { x,y,z }; };
    DirectX::XMFLOAT3 GetPos() const { return mPos; };

    // Getter & Setter for velocity
    void SetVelocity(const DirectX::XMFLOAT3& _velocity) { velocity = _velocity; }
    void SetVelocity(float x, float y, float z) { velocity = { x, y, z }; }
    DirectX::XMFLOAT3 GetVelocity() const { return velocity; }

    // Getter & Setter for acceleration
    void SetAcceleration(const DirectX::XMFLOAT3& _acceleration) { acceleration = _acceleration; }
    void SetAcceleration(float x, float y, float z) { acceleration = { x, y, z }; }
    DirectX::XMFLOAT3 GetAcceleration() const { return acceleration; }

    // Getter & Setter for force
    void SetForce(const DirectX::XMFLOAT3& _force) { force = _force; }
    void SetForce(float x, float y, float z) { force = { x, y, z }; }
    DirectX::XMFLOAT3 GetForce() const { return force; }

    // Getter & Setter for gravity
    void SetGravity(const DirectX::XMFLOAT3& _gravity) { gravity = _gravity; }
    void SetGravity(float x, float y, float z) { gravity = { x, y, z }; }
    DirectX::XMFLOAT3 GetGravity() const { return gravity; }

    // Getter & Setter for isPhysics
    void SetPhysicsEnabled(bool _isPhysics) { isPhysics = _isPhysics; }
    bool IsPhysicsEnabled() const { return isPhysics; }

};

