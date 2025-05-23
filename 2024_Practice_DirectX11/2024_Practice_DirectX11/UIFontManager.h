#pragma once
#include <SpriteFont.h>
#include "SpriteBatch.h"
#include <string>

//すべてのFontを入力・管理する
class UIFontManager
{
private:
	

	std::unique_ptr<DirectX::SpriteBatch> mBatch;
	std::unique_ptr<DirectX::SpriteFont> mCourierNewFont;


	UIFontManager() {};
	~UIFontManager() {};


public:

	static UIFontManager* Get()
	{
		static UIFontManager instance; return &instance;
	};

	void InitFontManager();

	DirectX::SpriteFont* GetCourierNew() { return mCourierNewFont.get(); };
	DirectX::SpriteBatch* GetBatch() { return mBatch.get(); };

	void DrawString();
};

