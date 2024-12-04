#include "UIStackContainer.h"
#include <memory>
#include "GUI.h"
#include "UI_Capsule.h"
#include "UI_Font.h"


UIStackContainer::UIStackContainer()
{
}

void UIStackContainer::InitUIStackContainer(UIPrimitiveConfig::UI_PrimitiveKind kind)
{
	mKind = kind;
	switch (mKind)
	{
	case UIPrimitiveConfig::UI_PrimitiveKind::SQUARE:
		mUiSet.mBackGround = std::make_unique<UI_Square>();
		break;

	case UIPrimitiveConfig::UI_PrimitiveKind::CAPSULE:
		mUiSet.mBackGround = std::make_unique<UI_Capsule>();
		break;
	}

}

void UIStackContainer::LoadBackgroundTex(const char* filePath, DirectX::XMFLOAT2 size)
{
	switch (mKind)
	{
	case UIPrimitiveConfig::UI_PrimitiveKind::CAPSULE:
		mUiSet.mBackGround->Init(filePath, size);
		SetContainerSize(1.0f);
		break;
	case UIPrimitiveConfig::UI_PrimitiveKind::SQUARE:
		mUiSet.mBackGround->Init(filePath);
		SetContainerSize(size);
		break;
	}
	mUIContainerSize = size;
	mUiSet.mBackGround->LoadDefShader();
}

void UIStackContainer::LoadBackgroundTex(const std::shared_ptr<Texture>& backgroundTex, DirectX::XMFLOAT2 size)
{

	switch(mKind)
	{
	case UIPrimitiveConfig::UI_PrimitiveKind::CAPSULE:
		mUiSet.mBackGround->Init(backgroundTex, size);
		SetContainerSize(1.0f);//Capsule‚Ìê‡ Scale‚Å‚Í‚È‚­’¸“_‚ð•ÏX‚µ‚Ä‚¢‚é
		break;
	case UIPrimitiveConfig::UI_PrimitiveKind::SQUARE:
		mUiSet.mBackGround->Init(backgroundTex, 0, { 1,1 });
		SetContainerSize(size);
		break;
	}
	
	mUIContainerSize = size;
	mUiSet.mBackGround->LoadDefShader();
}


void UIStackContainer::LoadFontTexture(const char* filePath,DirectX::XMFLOAT2 fontSize)
{
	mUiSet.mText = std::make_unique<UI_Font>();
	mUiSet.mText->Init(filePath, fontSize);
	isUseText = true;
}

void UIStackContainer::LoadFontTexture(const std::shared_ptr<Texture>& fontTex, DirectX::XMFLOAT2 fontSize)
{
	if (!fontTex)
	{
		isUseText = false;
		return;
	}

	mUiSet.mText = std::make_unique<UI_Font>();
	mUiSet.mText->Init(fontTex,fontSize);
	isUseText = true;
}

void UIStackContainer::Update()
{
	DebugFunction();
	if(isUseText)
	{
		UITextOption::AnchorAlign prevAlign = mUiSet.mText->GetAnchorAlign();
		mUiSet.mText->Update();
		if (prevAlign != mUiSet.mText->GetAnchorAlign())
			NotifyWordBoxChangeListener();
	}

	mUiSet.mBackGround->Update();

	UpdateContainerSize();
	UpdateContainerPosition();
	UpdateWordBox();
}

void UIStackContainer::Draw()
{
	if (!mUiSet.mBackGround)
		DebugLog::LogWarning("pBackground is nullptr");
	else
		mUiSet.mBackGround->Draw();


	if (!mUiSet.mText)
	{
		if (isUseText)
			DebugLog::LogWarning("pText is nullptr");
	}
	else
	{
		mUiSet.mText->Draw();
	}
	
	
}

void UIStackContainer::SetText(const char* text)
{
	if(!mUiSet.mText)
		DebugLog::LogWarning("pText is nullptr");
	else
		mUiSet.mText->SetContent(text);
}

void UIStackContainer::SetPosZ(float posZ)
{
	mPosition.z = posZ;
	mUiSet.mBackGround->SetPosZ(mPosition.z);

	if(isUseText)
		mUiSet.mText->SetPosZ(mPosition.z - 0.1f);
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

	mUiSet.mBackGround->SetDiffuseColor(color);
}

void UIStackContainer::SetFontColor(DirectX::XMFLOAT4 color)
{
	mUiSet.mText->SetFontColor(color);
}

void UIStackContainer::DebugFunction()
{
#ifdef _DEBUG 
	if (ImGui::Begin(mObjName.c_str()))
	{
		float pos[2] = { mPosition.x,mPosition.y };
		ImGui::InputFloat2("Position", pos);
		SetPosition(pos[0], pos[1]);

		float layerPosZ = mPosition.z;
		ImGui::InputFloat("LayerPosZ", &layerPosZ);
		SetPosZ(layerPosZ);

		float wordBoxSize[2] = { mUIContainerSize.x,mUIContainerSize.y };
		ImGui::InputFloat2("WordBoxSize", wordBoxSize);
		SetContainerSize(wordBoxSize[0], wordBoxSize[1]);

		float padding[4] = { mPadding[0],mPadding[1],mPadding[2],mPadding[3] };
		ImGui::InputFloat4("Padding", padding);
		SetPadding(padding);

		float bgColor[4] = { mUiSet.mBackGround->GetDiffuseColor().x,mUiSet.mBackGround->GetDiffuseColor().y,mUiSet.mBackGround->GetDiffuseColor().z,mUiSet.mBackGround->GetDiffuseColor().w };
		ImGui::ColorEdit4("Background", bgColor);
		mUiSet.mBackGround->SetDiffuseColor(Color(bgColor[0], bgColor[1], bgColor[2], bgColor[3]));

		//TextŽg‚í‚È‚¢Žž‚Íì“®‚µ‚È‚¢‚æ‚¤‚É
		if (isUseText)
		{
			float textColor[4] = { mUiSet.mText->GetFontColor().x,mUiSet.mText->GetFontColor().y,mUiSet.mText->GetFontColor().z,mUiSet.mText->GetFontColor().w };
			ImGui::ColorEdit4("TextColor", textColor);
			mUiSet.mText->SetFontColor(Color(textColor[0], textColor[1], textColor[2], textColor[3]));

			ImGui::Text("AnchorPos");
			GUI::ShowFloat3(mUiSet.mText->GetAnchorPos());

			ImGui::InputTextMultiline("Input your Text", mInputText, IM_ARRAYSIZE(mInputText));


			if (ImGui::Button("UpdateString"))
			{
				mUiSet.mText->SetContent(mInputText);
			}

			mUiSet.mText->DebugFunction();
		}

		ImGui::InputText("Input template Name", mTemplateName, IM_ARRAYSIZE(mTemplateName));

		if(ImGui::Button("Save the template"))
		{
			json data;
			data[mTemplateName] = SaveData(mTemplateName);
			std::ofstream file("Assets/Data/SaveDat/UITemplate/UITemplate.json");
			if (file.is_open())
			{
				file << data.dump(4);
				file.close();
			}
			else
			{
				DebugLog::Log("Failed to open jsonFile for writing.");
			}
		}

	}
	ImGui::End();


#endif
}

json UIStackContainer::SaveData(const char* objName)
{
	json data;
	//Position
	data["Position"] = { mPosition.x,mPosition.y,mPosition.z };
	//ContainerSize //BackGroundScale
	data["ContainerSize"] = { mUIContainerSize.x,mUIContainerSize.y };
	//Rotation
	data["Rotation"]={mUiSet.mBackGround->GetRotation().x,mUiSet.mBackGround->GetRotation().y,mUiSet.mBackGround->GetRotation().z };

	//bgColor
	data["Material"]["Ambient"] = { mUiSet.mBackGround->GetMaterial().ambient.x,mUiSet.mBackGround->GetMaterial().ambient.y, mUiSet.mBackGround->GetMaterial().ambient.z, mUiSet.mBackGround->GetMaterial().ambient.w };
	data["Material"]["Diffuse"] = { mUiSet.mBackGround->GetDiffuseColor().x,mUiSet.mBackGround->GetDiffuseColor().y,mUiSet.mBackGround->GetDiffuseColor().z,mUiSet.mBackGround->GetDiffuseColor().w }; 
	data["Material"]["Specular"] = { mUiSet.mBackGround->GetMaterial().specular.x,mUiSet.mBackGround->GetMaterial().specular.y,mUiSet.mBackGround->GetMaterial().specular.z,mUiSet.mBackGround->GetMaterial().specular.w };
	data["Material"]["Emission"] = { mUiSet.mBackGround->GetMaterial().emission.x,mUiSet.mBackGround->GetMaterial().emission.y, mUiSet.mBackGround->GetMaterial().emission.z, mUiSet.mBackGround->GetMaterial().emission.w };

	data["IsUseText"] = { (int)isUseText };
	if (!isUseText)
		return data;

	//Padding
	data["Padding"] = { mPadding[0],mPadding[1],mPadding[2],mPadding[3] };
	//fontColor
	data["FontColor"] = { mUiSet.mText->GetFontColor().x,mUiSet.mText->GetFontColor().y,mUiSet.mText->GetFontColor().z,mUiSet.mText->GetFontColor().w };
	//context
	data["Context"] = mUiSet.mText->GetContext().c_str();
	//Text Align
	data["TextAlign"] = {(int)mUiSet.mText->GetTextAlign()};
	//Text Anchor Align
	data["AnchorAlign"] = { (int)mUiSet.mText->GetAnchorAlign() };
	//Text Anchor Pos
	data["AnchorPosition"] = { mUiSet.mText->GetAnchorPos().x,mUiSet.mText->GetAnchorPos().y };
	//Text FontSize
	data["FontSize"] = { mUiSet.mText->GetFontSize() };
	//Text Block Width
	data["FontBlockWidth"] = { mUiSet.mText->GetFontRectWidth() };
	//Text LineSpacing
	data["LineSpacing"] = { mUiSet.mText->GetLineSpacing()};
	return data;


}

void UIStackContainer::LoadSaveData(json data, const char* objName)
{
	//SetPos;
	float pos[3] = { data[objName]["Position"][0], data[objName]["Position"][1],data[objName]["Position"][2]};
	SetPosition(pos[0], pos[1]);
	SetPosZ(pos[2]);

	float wordBoxSize[2] = { data[objName]["ContainerSize"][0],data[objName]["ContainerSize"][1]};
	SetContainerSize(wordBoxSize[0], wordBoxSize[1]);

	float bgColor[4] = {data[objName]["Material"]["Diffuse"][0],data[objName]["Material"]["Diffuse"][1] ,data[objName]["Material"]["Diffuse"][2] ,data[objName]["Material"]["Diffuse"][3]};
	mUiSet.mBackGround->SetDiffuseColor(Color(bgColor[0], bgColor[1], bgColor[2], bgColor[3]));

	int useText = { data[objName]["IsUseText"][0]};
	isUseText = useText;

	mObjName = objName;
	if (!isUseText)return;
	//SetPadding
	float padding[4] = { data[objName]["Padding"][0],data[objName]["Padding"][1] ,data[objName]["Padding"][2] ,data[objName]["Padding"][3] };
	SetPadding(padding);
	//Set Context
	Color fontColor = { data[objName]["FontColor"][0],data[objName]["FontColor"][1],data[objName]["FontColor"][2],data[objName]["FontColor"][3] };
	SetFontColor(fontColor);
	//Contents
	std::string str = data[objName]["Context"];
	mUiSet.mText->SetContent(str.c_str());
	//TextAlign
	int textAlign = data[objName]["TextAlign"][0];
	mUiSet.mText->SetTextAlign(static_cast<UITextOption::TextAlign>(textAlign));
	//Text Anchor pattern
	int anchorAlign = data[objName]["AnchorAlign"][0];
	mUiSet.mText->SetAnchorAlign(static_cast<UITextOption::AnchorAlign>(anchorAlign));
	//Anchor Pos
	float anchorPos[2] = {data[objName]["AnchorPosition"][0],data[objName]["AnchorPosition"][1]};
	mUiSet.mText->SetAnchorPos(anchorPos[0], anchorPos[1]);
	//Font Block Width
	float blockWidth = data[objName]["FontBlockWidth"][0];
	mUiSet.mText->SetFontRectWidth(blockWidth);
	//LineSpacing
	mUiSet.mText->SetLineSpacing(data[objName]["LineSpacing"][0]);
	//FontSize
	float fontSize = data[objName]["FontSize"][0];
	mUiSet.mText->SetFontSize(fontSize);
	//Set Font

	NotifySizeChangeListener();
	mUiSet.mText->NotifyFontSizeChanged();
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
		mUiSet.mBackGround->SetScale(mUIContainerSize);
		ClearSizeChangeListener();
	}
}

void UIStackContainer::UpdateContainerPosition()
{
	if (isContainerPosChange || gD3D->GetResized())
	{
		mUiSet.mBackGround->SetPosition(mPosition.x, mPosition.y);
		SetPosZ(mPosition.z);
		ClearPositionChangeListener();
	}
}

void UIStackContainer::UpdateWordBox()
{
	if (!mUiSet.mText)return;
	UpdateWordBoxSize();
	UpdateWordPosition();
}

void UIStackContainer::UpdateWordBoxSize()
{
	//if there need any word box size change
	float boxWidth = mUIContainerSize.x - mPadding[UIContainerConfig::LEFT] - mPadding[UIContainerConfig::RIGHT];
	if(isWordAdaptive)
		mUiSet.mText->SetFontRectWidth(boxWidth);
}

void UIStackContainer::UpdateWordPosition()
{
	if (!GetWordBoxChange()) return;

	float anchorPosX = 0.0f;
	float anchorPosY = 0.0f;
	switch (mUiSet.mText->GetAnchorAlign())
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
	mUiSet.mText->SetAnchorPos(anchorPosX, anchorPosY);
	ClearPositionChangeListener();
}

void UIStackContainer::TriggerListener()
{
}
