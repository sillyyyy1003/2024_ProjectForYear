#include "UIFont.h"

#include "Geometry.h"

UIFont::UIFont()
{
}

void UIFont::Init(const char* filePath,DirectX::XMFLOAT2 fontSize)
{
	mSize = fontSize;
	mOriginalSize = mSize;
	mText.resize(UITextOption::defaultMaxChar);

    //Init MaterialData
    mTexData.material = {
        Color(1.0f, 1.0f, 1.0f, 1.0f),		// 環境光
        Color(1.0f, 1.0f, 1.0f, 1.0f),		// 表面色
        Color(1.0f, 0.5f, 0.5f, 0.2f),		// 鏡面反射: specular power 1
        Color(0.0f, 0.0f, 0.0f, 0.0f)		// 自発光なし};
    };
    mTexData.tex = std::make_shared<Texture>();
    mTexData.tex->Create(filePath);

	for (int i = 0; i < UITextOption::defaultMaxChar; i++)
	{
		mText[i] = std::make_shared<CanvasUI>();
		//mText[i]->Init(filePath, UITextOption::defaultSplit);
        mText[i]->Init(mTexData, UITextOption::defaultSplit);
		mText[i]->InitCanvasSize(mSize.x,mSize.y);
	}

#ifdef _DEBUG

    mDebugAnchorPos = std::make_unique<CanvasUI>();
    mDebugAnchorPos->Init(nullptr);

#endif

}

void UIFont::SetFontRectWidth(float width)
{
    if(!isSetWidth)
    {
        mFontWidthRectWidth = width;
        isSetWidth = true;
    }

}


void UIFont::UpdateChar(const char* str, UITextOption::TextAlign align, UITextOption::AnchorAlign anchor)
{
    /*
    int charNum = static_cast<int>(strlen(str));

    //calculate total width & height
    float totalWidth = 0.0f;
    float lineHeight = mSize.y;
    float totalHeight = lineHeight;
    float lineWidth = 0.0f;//current lineWidth

    int lineCount = 1;

    for (int i = 0; i < charNum; i++)
    {
        if (str[i] == '\n' || (lineWidth + mSize.x > mFontWidthRectWidth && lineWidth > 0))
        {
        	totalHeight += lineHeight; //totalHeightを加算
            totalWidth = std::max(totalWidth, lineWidth);
            lineWidth = 0.0f;//Reset Line Width
            lineCount++;
        }

        lineWidth += mSize.x;
    }

    totalWidth = std::max(totalWidth, lineWidth);  //last line

	// Set Anchor pos
    float anchorPos.x = 0.0f, anchorPos.y = 0.0f;
    switch (anchor)
    {
    case UITextOption::AnchorAlign::TopLeft:      anchorPos.x = 0.0f; anchorPos.y = 0.0f; break;
    case UITextOption::AnchorAlign::TopCenter:    anchorPos.x = -totalWidth / 2; anchorPos.y = 0.0f; break;
    case UITextOption::AnchorAlign::TopRight:     anchorPos.x = -totalWidth; anchorPos.y = 0.0f; break;
    case UITextOption::AnchorAlign::CenterLeft:   anchorPos.x = 0.0f; anchorPos.y = totalHeight / 2; break;
    case UITextOption::AnchorAlign::Center:       anchorPos.x = -totalWidth / 2; anchorPos.y = totalHeight / 2; break;
    case UITextOption::AnchorAlign::CenterRight:  anchorPos.x = -totalWidth; anchorPos.y = totalHeight / 2; break;
    case UITextOption::AnchorAlign::BottomLeft:   anchorPos.x = 0.0f; anchorPos.y = totalHeight; break;
    case UITextOption::AnchorAlign::BottomCenter: anchorPos.x = -totalWidth / 2; anchorPos.y = totalHeight; break;
    case UITextOption::AnchorAlign::BottomRight:  anchorPos.x = -totalWidth; anchorPos.y = totalHeight; break;
    }

    // Draw the chars
    float currentX = anchorPos.x;
    float currentY = anchorPos.y;
    int lineCharCount = 0;

    for (int i = 0; i < charNum; i++)
    {
        char c = str[i];

		if (c == '\n')
        {
            currentX = anchorPos.x;      
            currentY -= lineHeight; 
            lineCharCount = 0;
            continue;//Skip this loop to avoid the /n being drawn
        }

        if(currentX + mSize.x > anchorPos.x + mFontWidthRectWidth && lineCharCount > 0)
        {
            currentX = anchorPos.x;
            currentY -= lineHeight;
            lineCharCount = 0;
        
        }

        // 根据 TextAlign 调整每行的水平对齐方式
        if (lineCharCount == 0)
        {
            switch (align)
            {
            case UITextOption::TextAlign::Left:   currentX = anchorPos.x; break;
            case UITextOption::TextAlign::Center: currentX = anchorPos.x + (totalWidth - lineWidth) / 2; break;
            case UITextOption::TextAlign::Right:  currentX = anchorPos.x + (totalWidth - lineWidth); break;
            }
        }

        //Set FrameX/FrameY
        int frameX = ((int)c - 32) % UITextOption::defaultSplit.x;
        mText[i]->GetUVAnimation()->SetFrameX(frameX);

        int frameY = ((int)c - 32) / UITextOption::defaultSplit.x;
        mText[i]->GetUVAnimation()->SetFrameY(frameY);

        // Set Char Pos
        mText[i]->mTransform.SetPosition(currentX, currentY, 0.1f);

        // Update Next Char
        currentX += mSize.x;
        lineCharCount++;

        //Update UV Animation
        mText[i]->Update();
    }
	*/

}

void UIFont::UpdateCharSize() noexcept
{
    //When the screen is resized
    if(gD3D->GetResized())
    {
        Vector2 ratio={0,0};
        //Get Change Ratio
        ratio.x = gD3D->GetWinWidth() / WIN_WIDTH;
        ratio.y = gD3D->GetWinHeight()/ WIN_HEIGHT;

        //Calculate Size
        mSize = mOriginalSize * ratio;
        //Calculate anchor pos
        mAnchorPos = mOriginalPos * ratio;
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
    int charNum = mContent.size();

    float currentLineWidth = 0.0f;
	float totalLineHeight = 0.0f;
    float totalLineWidth = 0.0;
    float lineHeight = mSize.y;
    int lineCount = 1;//行数
    
    // 计算行数和总宽度、高度
    for (int i = 0; i < charNum; i++)
    {
        // 换行处理
        if (mContent[i] == '\n' || (currentLineWidth + mSize.x) > mFontWidthRectWidth)
        {
            lineCount++;
            currentLineWidth = 0.0f;
            totalLineHeight += lineHeight;
        }
        currentLineWidth += mSize.x;
        totalLineWidth = std::max(totalLineWidth, currentLineWidth);  // 记录最宽的行宽
    }

    // 计算锚点起始位置
    float anchorX = mAnchorPos.x;
    float anchorY = mAnchorPos.y;

    switch (mAnchorAlign)
    {
    case UITextOption::AnchorAlign::TopLeft:
        // 使用原始的 anchorX 和 anchorY
        break;
    case UITextOption::AnchorAlign::TopCenter:
        anchorX -= totalLineWidth / 2;
        break;
    case UITextOption::AnchorAlign::TopRight:
        anchorX -= totalLineWidth;
        break;
    case UITextOption::AnchorAlign::CenterLeft:
        anchorY -= totalLineHeight / 2;
        break;
    case UITextOption::AnchorAlign::Center:
        anchorX -= totalLineWidth / 2;
        anchorY -= totalLineHeight / 2;
        break;
    case UITextOption::AnchorAlign::CenterRight:
        anchorX -= totalLineWidth;
        anchorY -= totalLineHeight / 2;
        break;
    case UITextOption::AnchorAlign::BottomLeft:
        anchorY -= totalLineHeight;
        break;
    case UITextOption::AnchorAlign::BottomCenter:
        anchorX -= totalLineWidth / 2;
        anchorY -= totalLineHeight;
        break;
    case UITextOption::AnchorAlign::BottomRight:
        anchorX -= totalLineWidth;
        anchorY -= totalLineHeight;
        break;
    }

    // 从锚点开始的字符位置更新
    float currentX = anchorX;
    float currentY = anchorY;
    currentLineWidth = 0.0f;

    for (int i = 0; i < charNum; i++)
    {
        char c = mContent[i];

        // 检测换行
        if (c == '\n' || (currentLineWidth + mSize.x) > mFontWidthRectWidth)
        {
            currentY += lineHeight;  // 换到下一行
            currentLineWidth = 0.0f;

            // 根据 TextAlign 调整行的起始位置
            switch (mTextAlign)
            {
            case UITextOption::TextAlign::Left:
                currentX = anchorX;
                break;
            case UITextOption::TextAlign::Center:
                currentX = anchorX + (mFontWidthRectWidth - currentLineWidth) / 2;
                break;
            case UITextOption::TextAlign::Right:
                currentX = anchorX + mFontWidthRectWidth - currentLineWidth;
                break;
            }
        }

        // 设置字符的实际位置
        mText[i]->mTransform.SetPosition(currentX, currentY, 0.1f);

        // 更新到下一个字符的位置
        currentX += mSize.x;
        currentLineWidth += mSize.x;
        //Update UV Animation
        mText[i]->Update();
    }
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

            ImGui::Text("Selected Text Align: %s", UITextOption::TextAlignToString(mTextAlign));
            ImGui::Text("Selected Anchor Align: %s", UITextOption::AnchorAlignToString(mAnchorAlign));
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
    mDebugAnchorPos->mTransform.SetPosition(mAnchorPos.x,mAnchorPos.y,0.1f);
#endif

}

void UIFont::Draw()
{
	for (int i = 0; i < mText.size(); i++)
	{
		mText[i]->Draw();
	}

#ifdef _DEBUG
    mDebugAnchorPos->Draw();
#endif
}

void UIFont::ResetAnchorPos(float x, float y)
{
    mAnchorPos = { x,y };
    mOriginalPos = mAnchorPos;
}

void UIFont::ResetAnchorPos(DirectX::XMFLOAT2 anchorPos)
{
    mAnchorPos = anchorPos;
    mOriginalPos = mAnchorPos;
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

void UIFont::ResetSize(DirectX::XMFLOAT2 _size)noexcept
{
    mSize = _size;
    mOriginalSize = mSize;

}
