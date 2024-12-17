#include "UIFontManager.h"

#include "D3DApp.h"

void UIFontManager::InitFontManager()
{
	mBatch = std::make_unique<DirectX::SpriteBatch>(gD3D->GetContext());
	mCourierNewFont = std::make_unique<DirectX::SpriteFont>(gD3D->GetDevice(), L"Assets/Font/SpriteFont/Courier_New.spritefont");
}

void UIFontManager::DrawString()
{
	mBatch->Begin();
	mCourierNewFont->DrawString(mBatch.get(), L"Hello!", DirectX::XMFLOAT2(0, 0));
	mBatch->End();
}
