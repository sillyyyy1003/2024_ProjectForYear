#pragma once
#include "SceneBase.h"
#include "Transform.h"
#include "Shader.h"

/// <summary>
/// ���b�V���̎��
/// </summary>
enum PrimitiveKind
{
	PRIMITIVE_DEFAULT,
	CUBE,
	CAPSULE,
	SPHERE,
	CYLINDER,
	PLANE,
};

/// <summary>
/// ��{���f���iCube/Sphere/Cone/Capsule...�j
/// </summary>
class Primitive:public Component
{
	//POINT mOldPos = {};
	//int mState = 0;//Debug�p
protected:
	struct MaterialData
	{
		Material material = {};
		std::unique_ptr<Texture> tex = nullptr;
	};

	VertexShader* mVS = nullptr;
	PixelShader* mPS = nullptr;

	std::shared_ptr<VertexShader> mDefVS = nullptr;
	std::shared_ptr<PixelShader> mDefPS = nullptr;

	bool isDefShader = true;

	PrimitiveKind mKind = PRIMITIVE_DEFAULT;
	MaterialData mMaterial = {};

public:

	/// @brief �傫���E�ʒu�E��]�Ȃǂ̏��
	Transform mTransform = {};


public:
	Primitive(PrimitiveKind kind);
	virtual ~Primitive();

	virtual void Init(const char* filePath = nullptr) = 0;
	virtual void Update(float dt);
	virtual void Draw(int texSlot = 0) = 0;

	virtual void SetScale(const DirectX::XMFLOAT3& scale) { mTransform.SetScale(scale); };
	virtual void SetScale(const DirectX::XMFLOAT2& scale) {};

	virtual DirectX::XMFLOAT3 GetPosition() const { return mTransform.GetPosition(); };
	virtual DirectX::XMFLOAT4 GetQuaternion() const { return mTransform.GetRotationQuat(); };
	virtual DirectX::XMFLOAT3 GetRotation() const { return mTransform.GetRotation(); };
	virtual DirectX::XMFLOAT3 GetScale() const { return mTransform.GetScale(); };

	
	virtual void SetTexUV(DirectX::XMFLOAT2 _texUV) {};

	virtual	void SetDiffuse(DirectX::XMFLOAT4 _diffuse) { mMaterial.material.diffuse = _diffuse; };
	virtual void SetMaterial(Material _material) { mMaterial.material = _material; };

	/// @brief Default Shader���g�����ǂ����H
	virtual void SetDefShader(bool isDefShader) { this->isDefShader = isDefShader; };
	virtual void SetPixelShader(PixelShader* ps) { mPS = ps; };
	virtual void SetVertexShader(VertexShader* vs) { mVS = vs; };

private:
	/// @brief �f�o�b�O�p�@�I�u�W�F�N�g��
	void UpdateState();

	/// @brief �I�u�W�F�N�g����]�����@
	/// @param mouseMove 
	/// @param dt 
	void UpdateTransform(DirectX::XMFLOAT2 mouseMove, float dt);
};

