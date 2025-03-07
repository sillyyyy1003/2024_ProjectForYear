﻿#include "D2D_UIStackContainer.h"

#include "D2D_UIRect.h"
#include "D2D_UIRoundedRect.h"
#include "MovableStaticObject.h"

D2D_UIStackContainer::D2D_UIStackContainer()
{
}

D2D_UIStackContainer::~D2D_UIStackContainer()
{
}

void D2D_UIStackContainer::Init(D2DUIConfig::UIShape shape, D2DUIConfig::FontSize size, const char* objName)
{
	switch (shape)
	{
	default:
	case D2DUIConfig::UIShape::RECT:
		mBackGround = std::make_unique<D2D_UIRect>();
		break;
	case D2DUIConfig::UIShape::ROUNDED_RECT:
		mBackGround = std::make_unique<D2D_UIRoundedRect>();
		break;
	}

	//SetFontSize
	mFontSize = size;

	//SetObjName
	mObjName = objName;
}

void D2D_UIStackContainer::Init(D2DUIConfig::UIShape shape, const char* objName)
{
	switch (shape)
	{
	default:
	case D2DUIConfig::UIShape::RECT:
		mBackGround = std::make_unique<D2D_UIRect>();
		break;
	case D2DUIConfig::UIShape::ROUNDED_RECT:
		mBackGround = std::make_unique<D2D_UIRoundedRect>();
		break;
	}
	//SetObjName
	mObjName = objName;
}


void D2D_UIStackContainer::Update(float dt)
{
#ifdef _DEBUG
	DebugFunction();
#endif

	if(isEmerging)
		EmergingFunction(dt);


}

void D2D_UIStackContainer::Draw()
{
	if (!isActive)return;

	if (mUiState & D2DUIConfig::STATE_USE_BACKGROUND)
	{
		//Draw the Background
		mBackGround->Draw();
	}

	if(mUiState& D2DUIConfig::STATE_USE_FONT)
	{
		//SetTextAlign
		D2DFont::Get()->SetTextAlign(mFontSize, mTextAlignment);

		//Draw the font
		float width = mScale.x - mPadding.x - mPadding.z;
		float height = mScale.y - mPadding.y - mPadding.w;
		//D2DFont::Get()->SetFontScale(mFontScale);
		D2DFont::Get()->DrawSolidText(mFontSize, mFontColor, mText.c_str(), mPosition, { width,height });
	}
}

void D2D_UIStackContainer::DrawWithRadianBrush()
{
	if (mUiState & D2DUIConfig::STATE_USE_BACKGROUND)
	{
		//Draw the Background
		mBackGround->DrawWithRadianBrush();
	}

	if (mUiState & D2DUIConfig::STATE_USE_FONT)
	{
		//SetTextAlign
		D2DFont::Get()->SetTextAlign(mFontSize, mTextAlignment);

		//Draw the font
		float width = mScale.x - mPadding.x - mPadding.z;
		float height = mScale.y - mPadding.y - mPadding.w;

		D2DFont::Get()->SetFontScale(mFontScale);
		D2DFont::Get()->DrawSolidText(mFontSize, mFontColor, mText.c_str(), mPosition, { width,height });
	}
}

void D2D_UIStackContainer::SetText(const char* text)
{
	mText.erase();
	mText = text;
}

void D2D_UIStackContainer::SetFontColor(D2D1::ColorF color)
{
	mFontColor = color;
}

json D2D_UIStackContainer::SaveData()
{
	json data;
	data["Graphic"] = mBackGround->SaveData();
	data["Text"] = mText.c_str();
	data["FontColor"] = { mFontColor.r,mFontColor.g,mFontColor.b,mFontColor.a };
	data["FontSize"] = static_cast<int>(mFontSize);
	data["Position"] = { mPosition.x,mPosition.y };
	data["Scale"] = { mScale.x,mScale.y };
	data["TextAlignment"] = static_cast<int>(mTextAlignment);
	data["Padding"] = { mPadding.x,mPadding.y,mPadding.z,mPadding.w };
	data["FontScale"] = { mFontScale.x,mFontScale.y };
	return data;
}

void D2D_UIStackContainer::LoadSaveData(json data)
{
	mBackGround->LoadSaveData(data,"Graphic");
	mText = data["Text"];
	mFontColor = { data["FontColor"][0],data["FontColor"][1] ,data["FontColor"][2] ,data["FontColor"][3] };
	mPosition = { data["Position"][0],data["Position"][1] };
	mScale = { data["Scale"][0],data["Scale"][1] };
	int num = { data["TextAlignment"]};
	mTextAlignment = static_cast<D2DUIConfig::TextAlignment>(num);
	mFontScale = { data["FontScale"][0],data["FontScale"][1] };
	mPadding = { data["Padding"][0],data["Padding"][1],data["Padding"][2],data["Padding"][3] };
}

void D2D_UIStackContainer::SetBackGroundColor(D2D1::ColorF color)
{
	mBackGround->SetBaseColor(color);
}

void D2D_UIStackContainer::EmergingFunction(float dt)
{
	if (mAccumulateTime < mDuration)
	{
		float easeStart = Ease::EaseOutSine(mAccumulateTime / mDuration);
		mAccumulateTime += dt;
		float easeEnd = Ease::EaseOutSine(mAccumulateTime / mDuration);
		float easeStep = easeEnd - easeStart;

		float alpha = 1.0f * easeStep;
		mFontColor.a += alpha;
		
		
	}else
	{
		//todo:make this to QT...
		mAccumulateTime = 0.0f;
		isEmerging = false;
	}


}

void D2D_UIStackContainer::InitEmergingFunc(float duration)
{
	mDuration = duration;
	mFontColor.a = 0.0f;
	isEmerging = true;
}



#ifdef _DEBUG
void D2D_UIStackContainer::DebugFunction()
{
	if(ImGui::Begin(mObjName.c_str()))
	{
		//SetPos
		float pos[2] = { mPosition.x,mPosition.y };
		ImGui::InputFloat2("Position", pos);
		mPosition = { pos[0], pos[1] };
		mBackGround->SetPosition(mPosition);

		//SetSize
		float scale[2] = { mScale.x,mScale.y };
		ImGui::InputFloat2("Scale", scale);
		mScale = { scale[0],scale[1] };
		mBackGround->SetScale(mScale);

		//SetBackGroundColor
		float bgColor[4] = {mBackGround->GetBackGroundColor().x,mBackGround->GetBackGroundColor().y ,mBackGround->GetBackGroundColor().z ,mBackGround->GetBackGroundColor().w};
		ImGui::ColorEdit4("BackGroundColor", bgColor);
		mBackGround->SetBaseColor({ bgColor[0],bgColor[1],bgColor[2],bgColor[3] });

		//Set radiusX
		float radiusX = mBackGround->GetRadiusX();
		ImGui::InputFloat("RadiusX", &radiusX);
		mBackGround->SetRadiusX(radiusX);

		//Set radiusY
		float radiusY = mBackGround->GetRadiusY();
		ImGui::InputFloat("RadiusY", &radiusY);
		mBackGround->SetRadiusY(radiusY);

		//SetFontColor
		float fontColor[4] = { mFontColor.r, mFontColor.g , mFontColor.b, mFontColor.a };
		ImGui::ColorEdit4("FontColor", fontColor);
		SetFontColor({ fontColor[0],fontColor[1],fontColor[2],fontColor[3] });

		//Set Text
		ImGui::InputTextMultiline("Input your Text", mInputText, IM_ARRAYSIZE(mInputText));
		if (ImGui::Button("UpdateString"))
		{
			SetText(mInputText);
		}

		float fontScale[2] = {mFontScale.x,mFontScale.y};
		ImGui::InputFloat2("FontScale", fontScale);
		mFontScale={fontScale[0],fontScale[1]};

		float padding[4] = { mPadding.x,mPadding.y,mPadding.z,mPadding.w };
		ImGui::InputFloat4("Padding", padding);
		mPadding = { padding[0],padding[1],padding[2],padding[3] };

		//Set OutlineColor
		float outlineColor[4] = { mBackGround->GetOutlineColor().x,mBackGround->GetOutlineColor().y,mBackGround->GetOutlineColor().z,mBackGround->GetOutlineColor().w };
		ImGui::ColorEdit4("OutlineColor", outlineColor);
		mBackGround->SetOutlineColor({ outlineColor[0],outlineColor[1],outlineColor[2],outlineColor[3] });
		//Set Outline weight
		float outlineWeight = mBackGround->GetOutlineWeight();
		ImGui::InputFloat("Outline", &outlineWeight);
		mBackGround->SetOutlineWeight(outlineWeight);

		//SetTextAlign
		const char* textAlignItems[] = { "Left", "Center", "Right","Justified"};
		int selectedTextAlign = static_cast<int>(mTextAlignment);
		if (ImGui::Combo("Text Align", &selectedTextAlign, textAlignItems, IM_ARRAYSIZE(textAlignItems)))
		{
			mTextAlignment = static_cast<D2DUIConfig::TextAlignment>(selectedTextAlign);
		}
	}
	ImGui::End();
}
#endif

void D2D_UIStackContainer::SetUIState(D2DUIConfig::UIState state)
{
	mUiState |= state;
}

void D2D_UIStackContainer::RemoveUIState(D2DUIConfig::UIState state)
{
	mUiState &= ~state;
}

void D2D_UIStackContainer::EnableAllState()
{
	mUiState |= D2DUIConfig::STATE_USE_FONT;
	mUiState |= D2DUIConfig::STATE_USE_BACKGROUND;
}
