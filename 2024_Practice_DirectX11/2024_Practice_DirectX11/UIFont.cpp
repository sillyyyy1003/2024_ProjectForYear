#include "UIFont.h"

#include "GampApp.h"
#include "Geometry.h"
#include "RenderState.h"

using namespace DirectX::SimpleMath;
using namespace DirectX;

UIFont::UIFont()
{
}



void UIFont::Init(const char* filePath,DirectX::XMFLOAT2 charSize)
{
	mCharSize = charSize;
    mOriginalCharSize = mCharSize;
	mText.resize(UITextOption::defaultMaxChar);

    mTexData.tex = std::make_shared<Texture>();
    mTexData.tex->Create(filePath);

	for (int i = 0; i < UITextOption::defaultMaxChar; i++)
	{
		mText[i] = std::make_shared<UI_Square>();
        mText[i]->Init(filePath,0, UITextOption::defaultSplit);
        mText[i]->SetScale(mCharSize.x, mCharSize.y);
        mText[i]->LoadDefShader("Assets/Shader/VS_DefaultUI.cso", "Assets/Shader/PS_UIFont.cso");
	}
#ifdef _DEBUG
    InitDebugFunction();
#endif


}

void UIFont::Init(std::shared_ptr<Texture>& fontTex, DirectX::XMFLOAT2 charSize)
{
    mCharSize = charSize;
    mOriginalCharSize = mCharSize;
    mText.resize(UITextOption::defaultMaxChar);

    mTexData.tex = fontTex;
    for (int i = 0; i < UITextOption::defaultMaxChar; i++)
    {
        mText[i] = std::make_shared<UI_Square>();
        mText[i]->Init(fontTex, 0, UITextOption::defaultSplit);
        mText[i]->SetScale(mCharSize.x, mCharSize.y);
        mText[i]->LoadDefShader("Assets/Shader/VS_DefaultUI.cso","Assets/Shader/PS_UIFont.cso");
    }

#ifdef _DEBUG
    InitDebugFunction();
#endif
}

void UIFont::SetFont(std::shared_ptr<Texture> fontTex)
{
    mTexData.tex.reset();
    mTexData.tex = fontTex;
    for(int i=0;i<UITextOption::defaultMaxChar;i++)
    {
        mText[i]->SetTexture(fontTex);
    }


}

void UIFont::SetFontRectWidth(float width)
{
    if (mBlockWidth == width)return;
	mBlockWidth = width;
	mOriginalBlockWidth = width;
}


void UIFont::UpdateCharSize() noexcept
{
    //When the screen is resized
    if(gD3D->GetResized()||GetFontSizeChanged())
    {
        Vector2 ratio={0,0};
        //Get Change Ratio
        ratio.x = gD3D->GetWinWidth() / WIN_WIDTH;
        ratio.y = gD3D->GetWinHeight()/ WIN_HEIGHT;

        //Calculate Size
        mCharSize = mOriginalCharSize * ratio * mFontSize;

        //Calculate winWidth
        mBlockWidth = mOriginalBlockWidth * ratio.x;

        //Calculate anchor pos //todo:根据不同的排列方式，anchor的变换方式也 不一样
        mAnchorPos = { mAnchorPos.x * ratio.x,mAnchorPos.y * ratio.y };

    }
}

void UIFont::UpdateContents(const char* str) noexcept
{
    mContent.erase();
    mContent = str;
}

void UIFont::UpdateContents(std::string str) noexcept
{
    mContent.erase();
    mContent = str;
}

void UIFont::UpdatePosition()
{

	int charNum = (int)mContent.size();
    float charWidth = mOriginalCharSize.x * mFontSize;
    float charHeight = mOriginalCharSize.y * mFontSize;
	// Calculate total width and height
    float lineHeight = mCharSize.y + mLineSpacing;
	float lineWidth = 0.0f; // current line width
	std::vector<float> lines; // store line widths for alignment

	for (int i = 0; i < charNum; i++)
	{
        if (mContent[i] == '\n')
        {
            lines.push_back(lineWidth);
            lineWidth = 0.0f; // Reset line width for the new line
            continue;
        }

	    if (lineWidth + charWidth > mBlockWidth && lineWidth > 0)
	    {
	        lineWidth = std::max(mBlockWidth, lineWidth);
	        lines.push_back(lineWidth);
	        lineWidth = 0.0f; // Reset line width for the new line
        }

	    lineWidth += charWidth;
	}

	lineWidth = std::min(mBlockWidth, lineWidth); // last line width
	lines.push_back(lineWidth); // store the last line width
    float totalHeight = lines.size() * mCharSize.y + lines.size() * mLineSpacing;


	// Set Anchor position
	switch (mAnchorAlign)
	{
    case UITextOption::AnchorAlign::TopLeft:
        startX = mAnchorPos.x;
        endX = mAnchorPos.x + mBlockWidth;
        startY = mAnchorPos.y;
        endY = mAnchorPos.y - totalHeight;
        break;
    case UITextOption::AnchorAlign::TopCenter:
        startX = mAnchorPos.x - mBlockWidth / 2;
        endX = startX + mBlockWidth;
        startY = mAnchorPos.y;
        endY = mAnchorPos.y - totalHeight;
        break;
    case UITextOption::AnchorAlign::TopRight:
        startX = mAnchorPos.x - mBlockWidth;
        endX = mAnchorPos.x;
        startY = mAnchorPos.y;
        endY = mAnchorPos.y - totalHeight;
        break;
    case UITextOption::AnchorAlign::CenterLeft:
        startX = mAnchorPos.x ;
        endX = mAnchorPos.x + mBlockWidth;
        startY = mAnchorPos.y + totalHeight / 2 ;
        endY = mAnchorPos.y - totalHeight / 2;
        break;
    case UITextOption::AnchorAlign::Center:
        startX = mAnchorPos.x - mBlockWidth / 2;
        endX = startX + mBlockWidth;
        startY = mAnchorPos.y + totalHeight / 2;
        endY = mAnchorPos.y - totalHeight / 2;
        break;
    case UITextOption::AnchorAlign::CenterRight:
        startX = mAnchorPos.x - mBlockWidth;
        endX = mAnchorPos.x;
        startY = mAnchorPos.y + totalHeight / 2;
        endY = mAnchorPos.y - totalHeight / 2;
		break;
    case UITextOption::AnchorAlign::BottomLeft:
        startX = mAnchorPos.x;
        endX = mAnchorPos.x + mBlockWidth;
        startY = mAnchorPos.y + totalHeight;
        endY = mAnchorPos.y;
        break;
    case UITextOption::AnchorAlign::BottomCenter:
        startX = mAnchorPos.x - mBlockWidth / 2 ;
        endX = mAnchorPos.x + mBlockWidth / 2;
        startY = mAnchorPos.y + totalHeight;
        endY = mAnchorPos.y;
        break;
    case UITextOption::AnchorAlign::BottomRight:
        startX = mAnchorPos.x - mBlockWidth;
        endX = mAnchorPos.x;
        startY = mAnchorPos.y + totalHeight;
        endY = mAnchorPos.y;
		break;
	}

    // Adjust anchor position by half character size to counteract character center anchor
    float offsetX = charWidth / 2.0f;
    float offsetY = charHeight / 2.0f;
	// Draw the characters
    float currentX = 0;
    float currentY = startY - offsetY;
   
	int lineIndex = 0;//current line index
    int currentLineCharIndex = 0;

	for (int i = 0; i < charNum; i++)
	{
	    char c = mContent[i];

        // Newline or wrap to a new line
        if (c == '\n')
        {
            currentY -= lineHeight;
            currentLineCharIndex = 0;
            lineIndex++;
            continue;
        }

        if (currentLineCharIndex * charWidth + offsetX > mBlockWidth && lineIndex < lines.size())
        {
           
            currentY -= lineHeight;
            currentLineCharIndex = 0;
            lineIndex++;
        }

		switch (mTextAlign)
        {
        case UITextOption::TextAlign::Left:
            currentX = startX + offsetX + currentLineCharIndex * charWidth;
            break;
        case UITextOption::TextAlign::Center:
            currentX = (startX + endX) / 2 - lines[lineIndex] / 2 + offsetX + currentLineCharIndex * charWidth;
            break;
        case UITextOption::TextAlign::Right:
            currentX = endX - lines[lineIndex] + offsetX + currentLineCharIndex * charWidth;
            break;
        }

	    // Calculate frameX and frameY for character texture
	    int frameX = ((int)c - 32) % UITextOption::defaultSplit.x;
	    int frameY = ((int)c - 32) / UITextOption::defaultSplit.x;
	    mText[i]->GetAnimation()->SetFrameX(frameX);
	    mText[i]->GetAnimation()->SetFrameY(frameY);

	    // Set character position
        mText[i]->SetPosition({ currentX, currentY, posZ });
        mText[i]->SetScale({ charWidth,charHeight });

	    // Update next character position
        currentLineCharIndex++;
	    // Update UV Animation
	    mText[i]->Update();
	}
}

void UIFont::SetFontColor(DirectX::XMFLOAT4 color)
{
    for (int i = 0; i < mText.size(); i++)
	{
        mText[i]->SetDiffuseColor(color);
      
	}
}

void UIFont::InitDebugFunction()
{
    mDebugAnchorPos = std::make_unique<UI_Square>();
    mDebugAnchorPos->Init(nullptr);
    mDebugAnchorPos->SetScale(10, 10);
    mDebugAnchorPos->SetDiffuseColor(Color(1.0f, 0.0f, 0.0f, 0.4f));
    mDebugAnchorPos->LoadDefShader();

    mDebugRect = std::make_unique<UI_Square>();
    mDebugRect->Init(nullptr);
    mDebugRect->SetScale(UITextOption::defaultFontRectWidth, UITextOption::defaultFontRectWidth);
    mDebugRect->SetDiffuseColor(Color(0.2f, 0.2f, 0.2f, 0.4f));
    mDebugRect->LoadDefShader();
}

void UIFont::DebugFunction()
{
	#ifdef _DEBUG

	    if (ImGui::Begin("FontSet"))
	    {
            //Text Align
	    	const char* textAlignItems[] = { "Left", "Center", "Right" };
            int selectedTextAlign = static_cast<int>(mTextAlign);
            if (ImGui::Combo("Text Align", &selectedTextAlign, textAlignItems, IM_ARRAYSIZE(textAlignItems)))
            {
                mTextAlign = static_cast<UITextOption::TextAlign>(selectedTextAlign);
            }

            // Anchor Align
            const char* anchorAlignItems[] = {
                "Top Left", "Top Center", "Top Right",
                "Center Left", "Center", "Center Right",
                "Bottom Left", "Bottom Center", "Bottom Right"
            };
            int selectedAnchorAlign = static_cast<int>(mAnchorAlign);
            if (ImGui::Combo("Anchor Align", &selectedAnchorAlign, anchorAlignItems, IM_ARRAYSIZE(anchorAlignItems)))
            {
                mAnchorAlign = static_cast<UITextOption::AnchorAlign>(selectedAnchorAlign);
            }

            float fontSize = mFontSize;
            ImGui::InputFloat("FontSize",&mFontSize);
            if (fontSize != mFontSize)
                NotifyFontSizeChanged();

            ImGui::InputFloat("LineSpacing", &mLineSpacing);

            float blockSize = mBlockWidth;
            ImGui::InputFloat("BlockWidth", &blockSize);
            SetFontRectWidth(blockSize);

	    }

	    ImGui::End();
	#endif
}


void UIFont::Update()
{
    DebugFunction();
    UpdateCharSize();
    UpdatePosition();

#ifdef _DEBUG
    mDebugAnchorPos->SetPosition({ mAnchorPos.x,mAnchorPos.y,0.1f });
    mDebugAnchorPos->Update();

	float posX = (startX + endX) / 2;
	float posY = (startY + endY) / 2;

	float width = endX - startX;
    float height = startY - endY;
    mDebugRect->SetPosition({ posX, posY, posZ + 0.05f });
    mDebugRect->SetScale(width, height);
    mDebugRect->Update();

#endif

}

void UIFont::Draw()
{
    for (int i = 0; i < mText.size(); i++)
    {
        mText[i]->Draw();
    }

#ifdef _DEBUG
    GameApp::SetBlendState(RenderState::BSTransparent);
    mDebugAnchorPos->Draw();
    mDebugRect->Draw();
#endif
}

void UIFont::SetAnchorPos(float x, float y)
{
    mAnchorPos = { x,y };
    mOriginalAnchorPos = mAnchorPos;
}

void UIFont::SetAnchorPos(DirectX::XMFLOAT2 anchorPos)
{
    mAnchorPos = anchorPos;
    mOriginalAnchorPos = mAnchorPos;
}

void UIFont::SetContent(const char* str)
{
    mContent = str;
}

void UIFont::SetAnchorAlign(UITextOption::AnchorAlign align)
{
    mAnchorAlign = align;
}


void UIFont::SetTextAlign(UITextOption::TextAlign align)
{
    mTextAlign = align;
}

void UIFont::SetFontSize(DirectX::XMFLOAT2 _size)noexcept
{
    mCharSize = _size;
    mOriginalCharSize = mCharSize;

}
