#include "Transform.h"
using namespace DirectX::SimpleMath;
using namespace DirectX;

Transform::Transform(const DirectX::XMFLOAT3& _pos, const DirectX::XMFLOAT4& _rotation, const DirectX::XMFLOAT3& _scale)
{
	mPos = _pos;
	mRotation = _rotation;
	mScale = _scale;
}

const DirectX::XMFLOAT4X4 Transform::GetMatrixFX4()
{
	DirectX::XMVECTOR scaleVec = XMLoadFloat3(&mScale);
	DirectX::XMVECTOR quaternion = XMLoadFloat4(&mRotation);
	DirectX::XMVECTOR positionVec = XMLoadFloat3(&mPos);
	DirectX::XMMATRIX World = XMMatrixAffineTransformation(scaleVec, g_XMZero, quaternion, positionVec);
	World = XMMatrixTranspose(World);
	DirectX::XMFLOAT4X4 fMat;
	DirectX::XMStoreFloat4x4(&fMat, World);
	return fMat;
}

const DirectX::XMMATRIX Transform::GetMatrix()
{
	DirectX::XMVECTOR scaleVec = XMLoadFloat3(&mScale);
	DirectX::XMVECTOR quaternion = XMLoadFloat4(&mRotation);
	DirectX::XMVECTOR positionVec = XMLoadFloat3(&mPos);
	DirectX::XMMATRIX World = XMMatrixAffineTransformation(scaleVec, g_XMZero, quaternion, positionVec);
	World = XMMatrixTranspose(World);
	return World;
}

const DirectX::XMMATRIX Transform::GetIdentityMatrix()
{
	DirectX::XMFLOAT3 identity = { 1,1,1 };
	DirectX::XMVECTOR scaleVec = XMLoadFloat3(&identity);
	DirectX::XMVECTOR quaternion = XMLoadFloat4(&mRotation);
	DirectX::XMVECTOR positionVec = XMLoadFloat3(&mPos);
	DirectX::XMMATRIX World = XMMatrixAffineTransformation(scaleVec, g_XMZero, quaternion, positionVec);
	World = XMMatrixTranspose(World);
	return World;
}

DirectX::XMFLOAT3 Transform::GetRotationInRadian() const
{
	float sinX = 2 * (mRotation.w * mRotation.x - mRotation.y * mRotation.z);
	float sinY_cosX = 2 * (mRotation.w * mRotation.y + mRotation.x * mRotation.z);
	float cosY_cosX = 1 - 2 * (mRotation.x * mRotation.x + mRotation.y * mRotation.y);
	float sinZ_cosX = 2 * (mRotation.w * mRotation.z + mRotation.x * mRotation.y);
	float cosZ_cosX = 1 - 2 * (mRotation.x * mRotation.x + mRotation.z * mRotation.z);

	DirectX::XMFLOAT3 rotation={};
	// gimbal lock
	if (fabs(fabs(sinX) - 1.0f) < 1e-5f)
	{
		rotation.x = copysignf(DirectX::XM_PI / 2, sinX);
		rotation.y = 2.0f * atan2f(mRotation.y, mRotation.w);
		rotation.z = 0.0f;
	}
	else
	{
		rotation.x = asinf(sinX);
		rotation.y = atan2f(sinY_cosX, cosY_cosX);
		rotation.z = atan2f(sinZ_cosX, cosZ_cosX);
	}

	//return rotation;
	return rotation;

}

DirectX::XMFLOAT3 Transform::GetRotation() const
{
	Vector3 rotation = GetRotationInRadian();
	rotation *= 180.0f / DirectX::XM_PI;
	return rotation;
}

DirectX::XMFLOAT3 Transform::GetRightAxis() const
{
	XMMATRIX R = XMMatrixRotationQuaternion(XMLoadFloat4(&mRotation));
	XMFLOAT3 right;
	XMStoreFloat3(&right, R.r[0]);
	return right;
}

DirectX::XMVECTOR Transform::GetRightAxisXM() const
{
	DirectX::XMFLOAT3 right = GetRightAxis();
	return DirectX::XMLoadFloat3(&right);
}

DirectX::XMFLOAT3 Transform::GetUpAxis() const
{
	XMMATRIX R = XMMatrixRotationQuaternion(XMLoadFloat4(&mRotation));
	XMFLOAT3 up;
	XMStoreFloat3(&up, R.r[1]);
	return up;
}

DirectX::XMVECTOR Transform::GetUpAxisXM() const
{
	DirectX::XMFLOAT3 up = GetUpAxis();
	return DirectX::XMLoadFloat3(&up);
}

DirectX::XMFLOAT3 Transform::GetForwardAxis() const
{
	XMMATRIX R = XMMatrixRotationQuaternion(XMLoadFloat4(&mRotation));
	XMFLOAT3 forward;
	XMStoreFloat3(&forward, R.r[2]);
	return forward;
}

DirectX::XMVECTOR Transform::GetForwardAxisXM() const
{
	DirectX::XMFLOAT3 forward = GetForwardAxis();
	return DirectX::XMLoadFloat3(&forward);
}

DirectX::XMFLOAT4X4 Transform::GetLocalToWorldMatrix() const
{
	DirectX::XMFLOAT4X4 res;
	DirectX::XMStoreFloat4x4(&res, GetLocalToWorldMatrixXM());
	return res;
}

DirectX::XMMATRIX Transform::GetLocalToWorldMatrixXM() const
{
	DirectX::XMVECTOR scaleVec = XMLoadFloat3(&mScale);
	DirectX::XMVECTOR quaternion = XMLoadFloat4(&mRotation);
	DirectX::XMVECTOR positionVec = XMLoadFloat3(&mPos);
	DirectX::XMMATRIX World = XMMatrixAffineTransformation(scaleVec, g_XMZero, quaternion, positionVec);
	return World;

}

DirectX::XMFLOAT4X4 Transform::GetWorldToLocalMatrix() const
{
	DirectX::XMFLOAT4X4 res;
	DirectX::XMStoreFloat4x4(&res, GetWorldToLocalMatrixXM());
	return res;
}

DirectX::XMMATRIX Transform::GetWorldToLocalMatrixXM() const
{
	DirectX::XMMATRIX InvWorld = DirectX::XMMatrixInverse(nullptr, GetLocalToWorldMatrixXM());
	return InvWorld;
}

void Transform::SetScale(const DirectX::XMFLOAT3& scale)
{
	mScale = scale;
}

void Transform::SetScale(float x, float y, float z)
{
	mScale = Vector3(x, y, z);
}

void Transform::SetScale(const float* _scale)
{
	mScale = Vector3(_scale[0], _scale[1], _scale[2]);
}

void Transform::SetScaleXZ(float x, float z)
{
	mScale.x = x;
	mScale.z = z;
}

void Transform::SetScaleXZ(const DirectX::XMFLOAT2& scale)
{
}

void Transform::SetScaleXY(float x, float y)
{
	mScale.x = x;
	mScale.y = y;
}

void Transform::SetScaleXY(const DirectX::XMFLOAT2& scale)
{
	mScale.x = scale.x;
	mScale.y = scale.y;
}

void Transform::SetRotationInDegree(const DirectX::XMFLOAT3& eulerAnglesInDegree)
{
	XMFLOAT3 radian = Vector3(XMConvertToRadians(eulerAnglesInDegree.x), XMConvertToRadians(eulerAnglesInDegree.y), XMConvertToRadians(eulerAnglesInDegree.z));
	SetRotationInRadian(radian);
}

void Transform::SetRotationInRadian(const DirectX::XMFLOAT3& eulerAnglesInRadian)
{
	auto quat = DirectX::XMQuaternionRotationRollPitchYawFromVector(DirectX::XMLoadFloat3(&eulerAnglesInRadian));
	DirectX::XMStoreFloat4(&mRotation, quat);
	
}

void Transform::SetRotationInRadian(float x, float y, float z)
{
	auto quat = DirectX::XMQuaternionRotationRollPitchYaw(x, y, z);
	DirectX::XMStoreFloat4(&mRotation, quat);
}

void Transform::SetRotationInDegree(float x, float y, float z)
{
	auto quat = DirectX::XMQuaternionRotationRollPitchYaw(XMConvertToRadians(x), XMConvertToRadians(y), XMConvertToRadians(z));
	DirectX::XMStoreFloat4(&mRotation, quat);
}

void Transform::SetRotationInRadian(const float* rot)
{
	Vector3 rotation = { rot[0],rot[1],rot[2] };
	SetRotationInRadian(rotation);
}

void Transform::SetRotationInDegree(const float* rot)
{
	Vector3 rotation = { rot[0],rot[1],rot[2] };
	SetRotationInDegree(rotation);
}


void Transform::SetPosition(const DirectX::XMFLOAT3& position)
{
	mPos = position;
}

void Transform::SetPosition(float x, float y, float z)
{
	mPos = Vector3(x, y, z);
}

void Transform::SetPosition(const float* pos)
{
	mPos = Vector3(pos[0], pos[1], pos[2]);
}

void Transform::SetPositionZ(float z)
{
	mPos.z = z;
}

void Transform::SetPosition(float x, float y)
{
	mPos.x = x;
	mPos.y = y;
}

void Transform::Rotate(const DirectX::XMFLOAT3& eulerAnglesInDegree)
{
	XMFLOAT3 radian = Vector3(XMConvertToRadians(eulerAnglesInDegree.x), XMConvertToRadians(eulerAnglesInDegree.y), XMConvertToRadians(eulerAnglesInDegree.z));

	auto newQuat = XMQuaternionRotationRollPitchYawFromVector(XMLoadFloat3(&radian));
	auto quat = XMLoadFloat4(&mRotation);
	XMStoreFloat4(&mRotation, XMQuaternionMultiply(quat, newQuat));
}


void Transform::RotateAxis(const DirectX::XMFLOAT3& axis, float degree)
{
	auto newQuat = XMQuaternionRotationAxis(XMLoadFloat3(&axis), XMConvertToRadians(degree));
	auto quat = XMLoadFloat4(&mRotation);
	XMStoreFloat4(&mRotation, XMQuaternionMultiply(quat, newQuat));
}

void Transform::RotateAround(const DirectX::XMFLOAT3& point, const DirectX::XMFLOAT3& axis, float degree)
{
	XMVECTOR quat = XMLoadFloat4(&mRotation);
	XMVECTOR positionVec = XMLoadFloat3(&mPos);
	XMVECTOR centerVec = XMLoadFloat3(&point);

	// 以point作为原点进行旋转
	XMMATRIX RT = XMMatrixRotationQuaternion(quat) * XMMatrixTranslationFromVector(positionVec - centerVec);
	RT *= XMMatrixRotationAxis(XMLoadFloat3(&axis),XMConvertToRadians(degree));
	RT *= XMMatrixTranslationFromVector(centerVec);
	XMStoreFloat4(&mRotation, XMQuaternionRotationMatrix(RT));
	XMStoreFloat3(&mPos, RT.r[3]);
}

void Transform::Translate(const DirectX::XMFLOAT3& direction, float magnitude)
{
	XMVECTOR directionVec = XMVector3Normalize(XMLoadFloat3(&direction));
	XMVECTOR newPosition = XMVectorMultiplyAdd(XMVectorReplicate(magnitude), directionVec, XMLoadFloat3(&mPos));
	XMStoreFloat3(&mPos, newPosition);
}

void Transform::LookAt(const DirectX::XMFLOAT3& target, const DirectX::XMFLOAT3& up)
{
	XMMATRIX View = XMMatrixLookAtLH(XMLoadFloat3(&mPos), XMLoadFloat3(&target), XMLoadFloat3(&up));
	XMMATRIX InvView = XMMatrixInverse(nullptr, View);
	XMStoreFloat4(&mRotation, XMQuaternionRotationMatrix(InvView));
}


void Transform::LookTo(const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT3& up)
{
	XMMATRIX View = XMMatrixLookToLH(XMLoadFloat3(&mPos), XMLoadFloat3(&direction), XMLoadFloat3(&up));
	XMMATRIX InvView = XMMatrixInverse(nullptr, View);
	XMStoreFloat4(&mRotation, XMQuaternionRotationMatrix(InvView));
}

DirectX::XMFLOAT3 Transform::GetEulerAnglesFromRotationMatrix(const DirectX::XMFLOAT4X4& rotationMatrix)
{
	DirectX::XMFLOAT3 rotation{};
	if (fabs(1.0f - fabs(rotationMatrix(2, 1))) < 1e-5f)
	{
		rotation.x = copysignf(DirectX::XM_PIDIV2, -rotationMatrix(2, 1));
		rotation.y = -atan2f(rotationMatrix(0, 2), rotationMatrix(0, 0));
		return rotation;
	}

	float c = sqrtf(1.0f - rotationMatrix(2, 1) * rotationMatrix(2, 1));
	if (isnan(c))
		c = 0.0f;

	rotation.z = atan2f(rotationMatrix(0, 1), rotationMatrix(1, 1));
	rotation.x = atan2f(-rotationMatrix(2, 1), c);
	rotation.y = atan2f(rotationMatrix(2, 0), rotationMatrix(2, 2));
	return rotation;
}

DirectX::XMFLOAT4X4 Transform::GenerateWorld() const
{
	return GetLocalToWorldMatrix();
}
