#include "CameraDCC.h"
#include "KInput.h"


enum CameraDCCKind
{
	///No Interaction
	CAM_DCC_NONE,
	///�O��
	CAM_DCC_ORBIT,
	///�ǐ�
	CAM_DCC_TRACK,
	///Zoom in/out
	CAM_DCC_DOLLY,
	///flight
	CAM_DCC_FLIGHT,
};

CameraDCC::CameraDCC()
{
}
CameraDCC::~CameraDCC()
{
}
void CameraDCC::Update()
{
	UpdateState();
	if (mState == CAM_DCC_NONE) return;

	Argument arg;
	// �}�E�X�ړ���
	POINT cursorPos;
	GetCursorPos(&cursorPos);
	arg.mouseMove = DirectX::XMFLOAT2((float)cursorPos.x - mOldPos.x, (float)cursorPos.y - mOldPos.y);
	mOldPos = cursorPos;
	// �J�������
	arg.vCamPos					= DirectX::XMLoadFloat3(&mPos);
	arg.vCamLook				= DirectX::XMLoadFloat3(&mFocus);
	DirectX::XMVECTOR vCamUp	= DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&mUpDir));
	DirectX::XMVECTOR vFront	= DirectX::XMVectorSubtract(arg.vCamLook, arg.vCamPos);
	// �J�����p��
	arg.vCamFront	= DirectX::XMVector3Normalize(vFront);
	arg.vCamSide	= DirectX::XMVector3Normalize(DirectX::XMVector3Cross(vCamUp, arg.vCamFront));
	arg.vCamUp		= DirectX::XMVector3Normalize(DirectX::XMVector3Cross(arg.vCamFront, arg.vCamSide));
	// �t�H�[�J�X����
	DirectX::XMStoreFloat(&arg.focus, DirectX::XMVector3Length(vFront));

	switch (mState)
	{
	case CAM_DCC_ORBIT:		UpdateOrbit(arg);	break;
	case CAM_DCC_TRACK:		UpdateTrack(arg);	break;
	case CAM_DCC_DOLLY:		UpdateDolly(arg);	break;
	case CAM_DCC_FLIGHT:	UpdateFlight(arg);	break;
	}
}
void CameraDCC::UpdateState()
{
	CameraDCCKind prev = (CameraDCCKind)mState;
	if (KInput::IsKeyPress(VK_MENU))
	{
		mState = CAM_DCC_NONE;
		if (KInput::IsKeyPress(VK_LBUTTON)) mState = CAM_DCC_ORBIT;
		if (KInput::IsKeyPress(VK_MBUTTON)) mState = CAM_DCC_TRACK;
		if (KInput::IsKeyPress(VK_RBUTTON)) mState = CAM_DCC_DOLLY;
	}
	else if (KInput::IsKeyPress(VK_RBUTTON))
	{
		mState = CAM_DCC_FLIGHT;
	}
	else
	{
		mState = CAM_DCC_NONE;
	}
	if (prev != mState)
	{
		GetCursorPos(&mOldPos);
	}
}
void CameraDCC::UpdateOrbit(Argument& arg)
{
	// �}�E�X�̈ړ��� / ��ʃT�C�Y �̔䗦����A��ʑS�̂łǂꂾ����]���邩�w�肷��B
	float angleX =  360.0f * arg.mouseMove.x / 1280.0f;
	float angleY =  180.0f * arg.mouseMove.y / 720.0f;

	// Y����]�ŃJ�����̉�������]
	DirectX::XMMATRIX matRotY = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(angleX));
	DirectX::XMVECTOR vAxis = DirectX::XMVector3Normalize(DirectX::XMVector3TransformCoord(arg.vCamSide, matRotY));

	// ��]��̎��ŏc��]
	DirectX::XMMATRIX matRotSide = DirectX::XMMatrixRotationAxis(vAxis, DirectX::XMConvertToRadians(angleY));
	DirectX::XMVECTOR vRelative = DirectX::XMVectorScale(arg.vCamFront, arg.focus);
	vRelative = DirectX::XMVector3TransformCoord(vRelative, matRotY * matRotSide);

	// �J�������W�X�V
	DirectX::XMStoreFloat3(&mPos, DirectX::XMVectorSubtract(arg.vCamLook, vRelative));

	// �A�b�v�x�N�g���X�V
	DirectX::XMStoreFloat3(&mUpDir, DirectX::XMVector3Normalize(DirectX::XMVector3Cross(DirectX::XMVector3Normalize(vRelative), vAxis)));

}
void CameraDCC::UpdateTrack(Argument& arg)
{
	// ����A�A���B�Ƃ���O�p�`�ɂ��� tan�� = A / B�����藧��
	// ��L�������ƂɊ���o�������i�̍������A�ړ��� / ��ʃT�C�Y �̔䗦����A�ړ��ʂƂ��ċ��߂�
	float farScreenHeight = tanf(mFovY * 0.5f) * mFarZ;
	float screenRateW = arg.mouseMove.x / 640.0f;
	float screenRateH = arg.mouseMove.y / 360.0f;
	float farMoveX = -farScreenHeight * screenRateW * mAspect;
	float farMoveY = farScreenHeight * screenRateH;

	// �J�����̎p�������ƂɈړ�
	float rate = arg.focus / mFarZ;
	DirectX::XMVECTOR vCamMove = DirectX::XMVectorZero();
	vCamMove = DirectX::XMVectorAdd(vCamMove, DirectX::XMVectorScale(arg.vCamSide, farMoveX * rate));
	vCamMove = DirectX::XMVectorAdd(vCamMove, DirectX::XMVectorScale(arg.vCamUp, farMoveY * rate));
	DirectX::XMStoreFloat3(&mPos, DirectX::XMVectorAdd(arg.vCamPos, vCamMove));
	DirectX::XMStoreFloat3(&mFocus, DirectX::XMVectorAdd(arg.vCamLook, vCamMove));
}
void CameraDCC::UpdateDolly(Argument& arg)
{
	float rate = arg.focus / mFarZ;
	DirectX::XMVECTOR vMove = DirectX::XMVectorScale(arg.vCamFront, mFarZ * 0.01f * rate * (arg.mouseMove.x + arg.mouseMove.y));
	DirectX::XMStoreFloat3(&mPos, DirectX::XMVectorAdd(arg.vCamPos, vMove));
}
void CameraDCC::UpdateFlight(Argument& arg)
{
	// �}�E�X�̈ړ��� / ��ʃT�C�Y �̔䗦����A��ʑS�̂łǂꂾ����]���邩�w�肷��B
	float angleX = 360.0f * arg.mouseMove.x / 1280.0f;
	float angleY = 180.0f * arg.mouseMove.y / 720.0f;

	// ����]
	DirectX::XMMATRIX matUpRot = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(angleX));
	DirectX::XMVECTOR vSideAxis = DirectX::XMVector3Normalize(DirectX::XMVector3TransformCoord(arg.vCamSide, matUpRot));

	// �c��]
	DirectX::XMMATRIX matSideRot = DirectX::XMMatrixRotationAxis(vSideAxis, DirectX::XMConvertToRadians(angleY));
	DirectX::XMVECTOR vFrontAxis = DirectX::XMVector3Normalize(DirectX::XMVector3TransformCoord(arg.vCamFront, matUpRot * matSideRot));

	// �L�[���͂ňړ�
	DirectX::XMVECTOR vCamMove = DirectX::XMVectorZero();
	if (KInput::IsKeyPress('W')) vCamMove = DirectX::XMVectorAdd(vCamMove, vFrontAxis);
	if (KInput::IsKeyPress('S')) vCamMove = DirectX::XMVectorSubtract(vCamMove, vFrontAxis);
	if (KInput::IsKeyPress('A')) vCamMove = DirectX::XMVectorSubtract(vCamMove, vSideAxis);
	if (KInput::IsKeyPress('D')) vCamMove = DirectX::XMVectorAdd(vCamMove, vSideAxis);
	if (KInput::IsKeyPress('Q')) vCamMove = DirectX::XMVectorAdd(vCamMove, DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
	if (KInput::IsKeyPress('E')) vCamMove = DirectX::XMVectorAdd(vCamMove, DirectX::XMVectorSet(0.0f,-1.0f, 0.0f, 0.0f));
	vCamMove = DirectX::XMVectorScale(vCamMove, 0.1f);

	// �X�V
	DirectX::XMVECTOR vCamPos = DirectX::XMVectorAdd(arg.vCamPos, vCamMove);
	DirectX::XMStoreFloat3(&mPos, vCamPos);
	DirectX::XMStoreFloat3(&mFocus, DirectX::XMVectorAdd(vCamPos, DirectX::XMVectorScale(vFrontAxis, arg.focus)));
	DirectX::XMStoreFloat3(&mUpDir, DirectX::XMVector3Normalize(DirectX::XMVector3Cross(vFrontAxis, vSideAxis)));
}