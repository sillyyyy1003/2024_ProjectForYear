#include "KInput.h"


KInput::~KInput()
{
}

HRESULT KInput::InitInput()
{
	// ˆê”ÔÅ‰‚Ì“ü—Í
	GetKeyboardState(Get()->mKeyTable);
	return S_OK;
}


void KInput::UpdateInput()
{
	// ŒÃ‚¢“ü—Í‚ðXV
	memcpy_s(Get()->mOldTable, sizeof(mOldTable), Get()->mKeyTable, sizeof(mKeyTable));
	// Œ»Ý‚Ì“ü—Í‚ðŽæ“¾
	GetKeyboardState(Get()->mKeyTable);
}

bool KInput::IsKeyPress(BYTE key)
{
	return Get()->mKeyTable[key] & 0x80;
}

bool KInput::IsKeyTrigger(BYTE key)
{
	return(Get()->mKeyTable[key] ^ Get()->mOldTable[key]) & Get()->mKeyTable[key] & 0x80;
}

bool KInput::IsKeyRelease(BYTE key)
{
	return (Get()->mKeyTable[key] ^ Get()->mOldTable[key]) & Get()->mOldTable[key] & 0x80;
}

bool KInput::IsKeyRepeat(BYTE key)
{
	return false;
}
