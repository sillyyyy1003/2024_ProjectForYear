#include "UIStackContainer.h"
#include <memory>

#include "GUI.h"
#include "UI_Capsule.h"

UIStackContainer::UIStackContainer()
{
}

void UIStackContainer::InitUIStackContainer(UIPrimitiveConfig::UI_PrimitiveKind kind)
{
	mKind = kind;
	switch (mKind)
	{
	case UIPrimitiveConfig::UI_PrimitiveKind::SQUARE:
		mBackGround = std::make_unique<UI_Square>();
		break;

	case UIPrimitiveConfig::UI_PrimitiveKind::CAPSULE:
		mBackGround = std::make_unique<UI_Capsule>();
		break;
	}

	mText = std::make_unique<UIFont>();
}

void UIStackContainer::LoadBackgroundTex(const char* filePath, DirectX::XMFLOAT2 size)
{
	switch (mKind)
	{
	case UIPrimitiveConfig::UI_PrimitiveKind::CAPSULE:
		mBackGround->Init(filePath, size);
		SetContainerSize(1);
		break;
	case UIPrimitiveConfig::UI_PrimitiveKind::SQUARE:
		mBackGround->Init(filePath);
		SetContainerSize(size);
		break;
	}

	mBackGround->LoadDefShader();
}

void UIStackContainer::LoadBackgroundTex(std::shared_ptr<Texture> backgroundTex, DirectX::XMFLOAT2 size)
{

	switch(mKind)
	{
	case UIPrimitiveConfig::UI_PrimitiveKind::CAPSULE:
		mBackGround->Init(backgroundTex, size);
		SetContainerSize(1.0f);//Capsule‚Ìê‡ Scale‚Å‚Í‚È‚­’¸“_‚ð•ÏX‚µ‚Ä‚¢‚é
		break;
	case UIPrimitiveConfig::UI_PrimitiveKind::SQUARE:
		mBackGround->Init(backgroundTex, 0, { 1,1 });
		SetContainerSize(size);
		break;
	}
	mBackGround->LoadDefShader();
	mUIContainerSize = size;
}


void UIStackContainer::LoadFontTexture(const char* filePath,DirectX::XMFLOAT2 fontSize)
{
	mText->Init(filePath, fontSize);

}

void UIStackContainer::LoadFontTexture(std::shared_ptr<Texture> fontTex, DirectX::XMFLOAT2 fontSize)
{
	mText->Init(fontTex,fontSize);
}

void UIStackContainer::Update()
{
#ifdef _DEBUG 
	if (ImGui::Begin("UIContainer Option"))
	{

		float pos[2] = { mPosition.x,mPosition.y };
		ImGui::InputFloat2("Position", pos);
		SetPosition(pos[0], pos[1]);

		bool isResetVertices = mBackGround->GetVerticesChange();
		ImGui::Checkbox("isResetVertices", &isResetVertices);
		mBackGround->SetResetVerticesData(isResetVertices);

		float wordBoxSize[2] = { mUIContainerSize.x,mUIContainerSize.y };
		ImGui::InputFloat2("WordBoxSize", wordBoxSize);
		SetContainerSize(wordBoxSize[0], wordBoxSize[1]);

		float padding[4] = { mPadding[0],mPadding[1],mPadding[2],mPadding[3] };
		ImGui::InputFloat4("Padding", padding);
		SetPadding(padding);

		float bgColor[4] = { mBackGround->GetDiffuseColor().x,mBackGround->GetDiffuseColor().y,mBackGround->GetDiffuseColor().z,mBackGround->GetDiffuseColor().w };
		ImGui::ColorEdit4("Background", bgColor);
		mBackGround->SetDiffuseColor(Color(bgColor[0], bgColor[1], bgColor[2], bgColor[3]));
		
		float textColor[4] = { mText->GetFontColor().x,mText->GetFontColor().y,mText->GetFontColor().z,mText->GetFontColor().w };
		ImGui::ColorEdit4("TextColor", textColor);
		mText->SetFontColor(Color(textColor[0], textColor[1], textColor[2], textColor[3]));

		ImGui::Text("AnchorPos");
		GUI::ShowFloat3(mText->GetAnchorPos());
		
	}
	ImGui::End();


#endif

	UITextOption::AnchorAlign prevAlign = mText->GetAnchorAlign();

	mBackGround->Update();
	mText->Update();

	if (prevAlign != mText->GetAnchorAlign())
		NotifyWordBoxChangeListener();

	UpdateContainerSize();
	UpdateContainerPosition();
	UpdateWordBox();
}

void UIStackContainer::Draw()
{
	if (!mBackGround)
		DebugLog::LogWarning("pBackground is nullptr");

	if (!mText)
		DebugLog::LogWarning("pText is nullptr");

	mBackGround->Draw();
	mText->Draw();
		
}

void UIStackContainer::SetText(const char* text)
{
	if(!mText)
		DebugLog::LogWarning("pText is nullptr");
	else
		mText->SetContent(text);
}

void UIStackContainer::SetPosZ(float posZ)
{
	mPosition.z = posZ;
	mBackGround->SetPosZ(mPosition.z);
	mText->SetPosZ(mPosition.z - 0.1f);
}

void UIStackContainer::SetPosition(float x, float y)
{
	if (mPosition.x == x && mPosition.y == y)return;
		
	NotifyPositionChangeListener();
	NotifyWordBoxChangeListener();
	mPosition.x = x;
	mPosition.y = y;
}

void UIStackContainer::InitPosition(float x, float y)
{
	mPosition.x = x;
	mPosition.y = y;
	NotifyPositionChangeListener();
	NotifyWordBoxChangeListener();
}

void UIStackContainer::SetContainerSize(float width, float height)
{
	if (mUIContainerSize.x == width && mUIContainerSize.y == height)
		return;

	NotifySizeChangeListener();
	NotifyWordBoxChangeListener();
	mUIContainerSize = { width,height };
	

}

void UIStackContainer::SetContainerSize(DirectX::XMFLOAT2 size)
{
	if (mUIContainerSize.x == size.x && mUIContainerSize.y == size.y)
		return;

	NotifySizeChangeListener();
	NotifyWordBoxChangeListener();
	mUIContainerSize = size;
	

}

void UIStackContainer::SetContainerSize(float size)
{
	if (mUIContainerSize.x == size && mUIContainerSize.y == size)
		return;

	NotifySizeChangeListener();
	NotifyWordBoxChangeListener();
	mUIContainerSize = { size,size };
}


void UIStackContainer::SetPadding(UIContainerConfig::Padding padding, float param)
{
	if (mPadding[padding] == param)return;

	NotifyWordBoxChangeListener();
	mPadding[padding] = param;
}

void UIStackContainer::SetPadding(float param)
{
	for (int i = 0; i < UIContainerConfig::PADDING_MAX; i++)
	{
		if (mPadding[i] != param)
		{
			NotifyWordBoxChangeListener();
		}

		mPadding[i] = param;
	}
}

void UIStackContainer::SetPadding(const float* param)
{
	for (int i = 0; i < UIContainerConfig::PADDING_MAX; i++)
	{
		if (mPadding[i] != param[i])
		{
			NotifyWordBoxChangeListener();
		}

		mPadding[i] = param[i];
	}
}

void UIStackContainer::SetBackgroundColor(DirectX::XMFLOAT4 color)
{
	mBackGround->SetDiffuseColor(color);
}

void UIStackContainer::SetFontColor(DirectX::XMFLOAT4 color)
{
	mText->SetFontColor(color);
}

void UIStackContainer::NotifyWordBoxChangeListener()
{
	isWordBoxChange = true;
}

void UIStackContainer::ClearWordBoxChangeListener()
{
	isWordBoxChange = false;
}


void UIStackContainer::NotifySizeChangeListener()
{
	isContainerSizeChange = true;
}

void UIStackContainer::ClearSizeChangeListener()
{
	isContainerSizeChange = false;
}

void UIStackContainer::NotifyPositionChangeListener()
{
	isContainerPosChange = true;
}

void UIStackContainer::ClearPositionChangeListener()
{
	isContainerPosChange = false;
}

void UIStackContainer::UpdateContainerSize()
{
	if(isContainerSizeChange || gD3D->GetResized())
	{
		mBackGround->SetScale(mUIContainerSize);
		ClearSizeChangeListener();
	}
}

void UIStackContainer::UpdateContainerPosition()
{
	if (isContainerPosChange || gD3D->GetResized())
	{
		mBackGround->SetPosition(mPosition.x, mPosition.y);
		SetPosZ(mPosition.z);
		ClearPositionChangeListener();
	}
}

void UIStackContainer::UpdateWordBox()
{
	UpdateWordBoxSize();
	UpdateWordPosition();
}

void UIStackContainer::UpdateWordBoxSize()
{
	//if there need any word box size change
	float boxWidth = mUIContainerSize.x - mPadding[UIContainerConfig::LEFT] - mPadding[UIContainerConfig::RIGHT];
	if(isWordAdaptive)
		mText->SetFontRectWidth(boxWidth);
}

void UIStackContainer::UpdateWordPosition()
{
	if (!GetWordBoxChange()) return;

	float anchorPosX = 0.0f;
	float anchorPosY = 0.0f;
	switch (mText->GetAnchorAlign())
	{
	default:
	case UITextOption::AnchorAlign::TopLeft:
		anchorPosX = mPosition.x - mUIContainerSize.x / 2.0f + mPadding[UIContainerConfig::LEFT];
		anchorPosY = mPosition.y + mUIContainerSize.y / 2.0f - mPadding[UIContainerConfig::TOP];
		break;

	case UITextOption::AnchorAlign::TopCenter:
		anchorPosX = mPosition.x;
		anchorPosY = mPosition.y + mUIContainerSize.y / 2.0f - mPadding[UIContainerConfig::TOP];
		break;

	case UITextOption::AnchorAlign::TopRight:
		anchorPosX = mPosition.x + mUIContainerSize.x / 2.0f - mPadding[UIContainerConfig::RIGHT];
		anchorPosY = mPosition.y + mUIContainerSize.y / 2.0f - mPadding[UIContainerConfig::TOP];
		break;

	case UITextOption::AnchorAlign::CenterLeft:
		anchorPosX = mPosition.x - mUIContainerSize.x / 2.0f + mPadding[UIContainerConfig::LEFT];
		anchorPosY = mPosition.y;
		break;

	case UITextOption::AnchorAlign::Center:
		anchorPosX = mPosition.x;
		anchorPosY = mPosition.y;
		break;

	case UITextOption::AnchorAlign::CenterRight:
		anchorPosX = mPosition.x + mUIContainerSize.x / 2.0f - mPadding[UIContainerConfig::RIGHT];
		anchorPosY = mPosition.y;
		break;

	case UITextOption::AnchorAlign::BottomLeft:
		anchorPosX = mPosition.x - mUIContainerSize.x / 2.0f + mPadding[UIContainerConfig::LEFT];
		anchorPosY = mPosition.y - mUIContainerSize.y / 2.0f + mPadding[UIContainerConfig::BOTTOM];
		break;

	case UITextOption::AnchorAlign::BottomCenter:
		anchorPosX = mPosition.x;
		anchorPosY = mPosition.y - mUIContainerSize.y / 2.0f + mPadding[UIContainerConfig::BOTTOM];
		break;

	case UITextOption::AnchorAlign::BottomRight:
		anchorPosX = mPosition.x + mUIContainerSize.x / 2.0f - mPadding[UIContainerConfig::RIGHT];
		anchorPosY = mPosition.y - mUIContainerSize.y / 2.0f + mPadding[UIContainerConfig::BOTTOM];
		break;
	}

	// Set the calculated anchor position
	mText->SetAnchorPos(anchorPosX, anchorPosY);
	ClearPositionChangeListener();
}

void UIStackContainer::TriggerListener()
{
}
