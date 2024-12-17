#include "UIStackContainer.h"
#include <memory>

#include "D2DFont.h"
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
		mUiSet.background = std::make_unique<UI_Square>();
		mUiSet.outline = std::make_unique<UI_Square>();
		break;

	case UIPrimitiveConfig::UI_PrimitiveKind::CAPSULE:
		mUiSet.background = std::make_unique<UI_Capsule>();
		mUiSet.outline = std::make_unique<UI_Capsule>();
		break;
	}

}

void UIStackContainer::LoadBackgroundTex(const char* filePath, DirectX::XMFLOAT2 size)
{
	switch (mKind)
	{
	case UIPrimitiveConfig::UI_PrimitiveKind::CAPSULE:
		mUiSet.background->Init(filePath, size);
		mUiSet.outline->Init(nullptr, size);
		SetContainerSize(1.0f);
		break;
	case UIPrimitiveConfig::UI_PrimitiveKind::SQUARE:
		mUiSet.background->Init(filePath);
		mUiSet.outline->Init(nullptr);
		SetContainerSize(size);
		break;
	}
	mUIContainerSize = size;
	mUiSet.background->LoadDefShader();
	mUiSet.outline->LoadDefShader();
}

void UIStackContainer::LoadBackgroundTex(const std::shared_ptr<Texture>& backgroundTex, DirectX::XMFLOAT2 size)
{

	switch(mKind)
	{
		default:
	case UIPrimitiveConfig::UI_PrimitiveKind::CAPSULE:
		mUiSet.background->Init(backgroundTex, size);
		mUiSet.outline->Init(nullptr, size);
		SetContainerSize(1.0f);//Capsule‚Ìê‡ Scale‚Å‚Í‚È‚­’¸“_‚ð•ÏX‚µ‚Ä‚¢‚é
		break;
	case UIPrimitiveConfig::UI_PrimitiveKind::SQUARE:
		mUiSet.background->Init(backgroundTex, 0, { 1,1 });
		mUiSet.outline->Init(nullptr, 0, { 1,1 });
		SetContainerSize(size);
		break;
	}
	
	mUIContainerSize = size;
	mUiSet.background->LoadDefShader();
	mUiSet.outline->LoadDefShader();
}


void UIStackContainer::LoadFontTexture(const char* filePath,DirectX::XMFLOAT2 fontSize)
{
	mUiSet.text = std::make_unique<UI_Font>();
	mUiSet.text->Init(filePath, fontSize);
	isUseText = true;
}

void UIStackContainer::LoadFontTexture(const std::shared_ptr<Texture>& fontTex, DirectX::XMFLOAT2 fontSize)
{
	if (!fontTex)
	{
		isUseText = false;
		return;
	}

	mUiSet.text = std::make_unique<UI_Font>();
	mUiSet.text->Init(fontTex,fontSize);
	isUseText = true;
}

void UIStackContainer::LoadFile(const std::shared_ptr<Texture>& bgTex,
	const std::shared_ptr<Texture>& fontTex, DirectX::XMFLOAT2 texSize, DirectX::XMFLOAT2 fontSize)
{
	LoadBackgroundTex(bgTex, texSize);
	LoadFontTexture(fontTex, fontSize);

}

void UIStackContainer::Update()
{
#ifdef _DEBUG
	if(isEditable)
		DebugFunction();
#endif

	if(isUseText)
	{
		UITextOption::AnchorAlign prevAlign = mUiSet.text->GetAnchorAlign();
		if (prevAlign != mUiSet.text->GetAnchorAlign())
			NotifyWordBoxChangeListener();
		mUiSet.text->Update();
	}

	mUiSet.background->Update();
	if(mOutline>0)mUiSet.outline->Update();

	UpdateContainerSize();
	UpdateContainerPosition();
	UpdateWordBox();
}

void UIStackContainer::Draw()
{
	if (!mUiSet.background)DebugLog::LogWarning("pBackground is nullptr");
	else mUiSet.background->Draw();


	//if (!mUiSet.outline)
	//{
	//	DebugLog::LogWarning("pOutline is nullptr");
	//}
	//else
	//{
	//	if (mOutline > 0)mUiSet.outline->Draw();
	//}


	//if (!mUiSet.text)
	//{
	//	if (isUseText)
	//		DebugLog::LogWarning("pText is nullptr");
	//}
	//else
	//{
	//	mUiSet.text->Draw();
	//}

	D2DFont::Get()->DrawSolidText(D2DUIConfig::FontSize::NORMAL_SIZE, D2D1::ColorF{ 1,1,1,1 }, mUiSet.text->GetContext().c_str(), { mPosition.x, mPosition.y }, mUIContainerSize);

}

void UIStackContainer::SetText(const char* text)
{
	if(!mUiSet.text)
		DebugLog::LogWarning("pText is nullptr");
	else
		mUiSet.text->SetContent(text);
}

void UIStackContainer::SetPosZ(float posZ)
{
	//Background
	mPosition.z = posZ;
	mUiSet.background->SetPosZ(mPosition.z);

	//Outline
	mUiSet.outline->SetPosZ(mPosition.z + 0.1f);

	//Text
	if(isUseText)
		mUiSet.text->SetPosZ(mPosition.z - 0.2f);
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
	mUiSet.background->SetDiffuseColor(color);
}

void UIStackContainer::SetOutlineColor(DirectX::XMFLOAT4 color)
{
	mUiSet.outline->SetDiffuseColor(color);
}

void UIStackContainer::SetFontColor(DirectX::XMFLOAT4 color)
{
	mUiSet.text->SetFontColor(color);
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

		float bgColor[4] = { mUiSet.background->GetDiffuseColor().x,mUiSet.background->GetDiffuseColor().y,mUiSet.background->GetDiffuseColor().z,mUiSet.background->GetDiffuseColor().w };
		ImGui::ColorEdit4("Background", bgColor);
		mUiSet.background->SetDiffuseColor(Color(bgColor));


		float outlineColor[4] = { mUiSet.outline->GetDiffuseColor().x,mUiSet.outline->GetDiffuseColor().y,mUiSet.outline->GetDiffuseColor().z,mUiSet.outline->GetDiffuseColor().w };
		ImGui::ColorEdit4("OutlineColor", outlineColor);
		mUiSet.outline->SetDiffuseColor(Color(outlineColor));

		ImGui::InputFloat("Outline", &mOutline);
		mUiSet.outline->SetScale({ mUIContainerSize.x + mOutline,mUIContainerSize.y + mOutline });

		//TextŽg‚í‚È‚¢Žž‚Íì“®‚µ‚È‚¢‚æ‚¤‚É
		if (isUseText)
		{
			float textColor[4] = { mUiSet.text->GetFontColor().x,mUiSet.text->GetFontColor().y,mUiSet.text->GetFontColor().z,mUiSet.text->GetFontColor().w };
			ImGui::ColorEdit4("TextColor", textColor);
			mUiSet.text->SetFontColor(Color(textColor[0], textColor[1], textColor[2], textColor[3]));

			ImGui::Text("AnchorPos");
			GUI::ShowFloat3(mUiSet.text->GetAnchorPos());

			ImGui::InputTextMultiline("Input your Text", mInputText, IM_ARRAYSIZE(mInputText));

			ImGui::InputFloat("MinFontWidth", &mMinBoxWidth);

			ImGui::InputFloat("MaxFontWidth", &mMaxBoxWidth);


			if (ImGui::Button("UpdateString"))
			{
				mUiSet.text->SetContent(mInputText);
			}

			mUiSet.text->DebugFunction();
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
	data["Rotation"]={mUiSet.background->GetRotation().x,mUiSet.background->GetRotation().y,mUiSet.background->GetRotation().z };

	//bgColor
	data["Material"]["Ambient"] = { mUiSet.background->GetMaterial().ambient.x,mUiSet.background->GetMaterial().ambient.y, mUiSet.background->GetMaterial().ambient.z, mUiSet.background->GetMaterial().ambient.w };
	data["Material"]["Diffuse"] = { mUiSet.background->GetDiffuseColor().x,mUiSet.background->GetDiffuseColor().y,mUiSet.background->GetDiffuseColor().z,mUiSet.background->GetDiffuseColor().w }; 
	data["Material"]["Specular"] = { mUiSet.background->GetMaterial().specular.x,mUiSet.background->GetMaterial().specular.y,mUiSet.background->GetMaterial().specular.z,mUiSet.background->GetMaterial().specular.w };
	data["Material"]["Emission"] = { mUiSet.background->GetMaterial().emission.x,mUiSet.background->GetMaterial().emission.y, mUiSet.background->GetMaterial().emission.z, mUiSet.background->GetMaterial().emission.w };

	data["OutlineColor"] = { mUiSet.outline->GetDiffuseColor().x,mUiSet.outline->GetDiffuseColor().y,mUiSet.outline->GetDiffuseColor().z,mUiSet.outline->GetDiffuseColor().w };

	data["Outline"] = { mOutline };

	data["IsUseText"] = { (int)isUseText };
	if (!isUseText)
		return data;

	//Padding
	data["Padding"] = { mPadding[0],mPadding[1],mPadding[2],mPadding[3] };
	//fontColor
	data["FontColor"] = { mUiSet.text->GetFontColor().x,mUiSet.text->GetFontColor().y,mUiSet.text->GetFontColor().z,mUiSet.text->GetFontColor().w };
	//context
	data["Context"] = mUiSet.text->GetContext().c_str();
	//Text Align
	data["TextAlign"] = {(int)mUiSet.text->GetTextAlign()};
	//Text Anchor Align
	data["AnchorAlign"] = { (int)mUiSet.text->GetAnchorAlign() };
	//Text Anchor Pos
	data["AnchorPosition"] = { mUiSet.text->GetAnchorPos().x,mUiSet.text->GetAnchorPos().y };
	//Text FontSize
	data["FontSize"] = { mUiSet.text->GetFontSize() };
	//Text Block Width
	data["FontBlockWidth"] = { mUiSet.text->GetFontRectWidth() };
	//Text LineSpacing
	data["LineSpacing"] = { mUiSet.text->GetLineSpacing()};
	return data;

}

json UIStackContainer::SaveData()
{
	json data;
	//Position
	data["Position"] = { mPosition.x,mPosition.y,mPosition.z };
	//ContainerSize //BackGroundScale
	data["ContainerSize"] = { mUIContainerSize.x,mUIContainerSize.y };
	//Rotation
	data["Rotation"] = { mUiSet.background->GetRotation().x,mUiSet.background->GetRotation().y,mUiSet.background->GetRotation().z };

	//bgColor
	data["Material"]["Ambient"] = { mUiSet.background->GetMaterial().ambient.x,mUiSet.background->GetMaterial().ambient.y, mUiSet.background->GetMaterial().ambient.z, mUiSet.background->GetMaterial().ambient.w };
	data["Material"]["Diffuse"] = { mUiSet.background->GetDiffuseColor().x,mUiSet.background->GetDiffuseColor().y,mUiSet.background->GetDiffuseColor().z,mUiSet.background->GetDiffuseColor().w };
	data["Material"]["Specular"] = { mUiSet.background->GetMaterial().specular.x,mUiSet.background->GetMaterial().specular.y,mUiSet.background->GetMaterial().specular.z,mUiSet.background->GetMaterial().specular.w };
	data["Material"]["Emission"] = { mUiSet.background->GetMaterial().emission.x,mUiSet.background->GetMaterial().emission.y, mUiSet.background->GetMaterial().emission.z, mUiSet.background->GetMaterial().emission.w };

	data["OutlineColor"] = { mUiSet.outline->GetDiffuseColor().x,mUiSet.outline->GetDiffuseColor().y,mUiSet.outline->GetDiffuseColor().z,mUiSet.outline->GetDiffuseColor().w };

	data["Outline"] = { mOutline };

	data["IsUseText"] = { (int)isUseText };
	if (!isUseText)
		return data;

	//Padding
	data["Padding"] = { mPadding[0],mPadding[1],mPadding[2],mPadding[3] };
	//fontColor
	data["FontColor"] = { mUiSet.text->GetFontColor().x,mUiSet.text->GetFontColor().y,mUiSet.text->GetFontColor().z,mUiSet.text->GetFontColor().w };
	//context
	data["Context"] = mUiSet.text->GetContext().c_str();
	//Text Align
	data["TextAlign"] = { (int)mUiSet.text->GetTextAlign() };
	//Text Anchor Align
	data["AnchorAlign"] = { (int)mUiSet.text->GetAnchorAlign() };
	//Text Anchor Pos
	data["AnchorPosition"] = { mUiSet.text->GetAnchorPos().x,mUiSet.text->GetAnchorPos().y };
	//Text FontSize
	data["FontSize"] = { mUiSet.text->GetFontSize() };
	//Text Block Width
	data["FontBlockWidth"] = { mUiSet.text->GetFontRectWidth() };
	//Text LineSpacing
	data["LineSpacing"] = { mUiSet.text->GetLineSpacing() };
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
	mUiSet.background->SetDiffuseColor(Color(bgColor[0], bgColor[1], bgColor[2], bgColor[3]));

	int useText = { data[objName]["IsUseText"][0]};
	isUseText = useText;

	mObjName = objName;

	float outlineColor[4] = { data[objName]["OutlineColor"][0],data[objName]["OutlineColor"][1],data[objName]["OutlineColor"][2],data[objName]["OutlineColor"][3] };
	mUiSet.outline->SetDiffuseColor(Color(outlineColor));

	mOutline = { data[objName]["Outline"][0] };


	if (!isUseText)return;
	//SetPadding
	float padding[4] = { data[objName]["Padding"][0],data[objName]["Padding"][1] ,data[objName]["Padding"][2] ,data[objName]["Padding"][3] };
	SetPadding(padding);
	//Set Context
	Color fontColor = { data[objName]["FontColor"][0],data[objName]["FontColor"][1],data[objName]["FontColor"][2],data[objName]["FontColor"][3] };
	SetFontColor(fontColor);
	//Contents
	std::string str = data[objName]["Context"];
	mUiSet.text->SetContent(str.c_str());
	//TextAlign
	int textAlign = data[objName]["TextAlign"][0];
	mUiSet.text->SetTextAlign(static_cast<UITextOption::TextAlign>(textAlign));
	//Text Anchor pattern
	int anchorAlign = data[objName]["AnchorAlign"][0];
	mUiSet.text->SetAnchorAlign(static_cast<UITextOption::AnchorAlign>(anchorAlign));
	//Anchor Pos
	float anchorPos[2] = {data[objName]["AnchorPosition"][0],data[objName]["AnchorPosition"][1]};
	mUiSet.text->SetAnchorPos(anchorPos[0], anchorPos[1]);
	//Font Block Width
	float blockWidth = data[objName]["FontBlockWidth"][0];
	mUiSet.text->SetFontRectWidth(blockWidth);
	//LineSpacing
	mUiSet.text->SetLineSpacing(data[objName]["LineSpacing"][0]);
	//FontSize
	float fontSize = data[objName]["FontSize"][0];
	mUiSet.text->SetFontSize(fontSize);
	//Set Font

	NotifySizeChangeListener();
	mUiSet.text->NotifyFontSizeChanged();
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
		mUiSet.background->SetScale(mUIContainerSize);
		Vector2 outlineSize = { mUIContainerSize.x + mOutline,mUIContainerSize.y + mOutline };
		mUiSet.outline->SetScale(outlineSize);
		ClearSizeChangeListener();
	}
}

void UIStackContainer::UpdateContainerPosition()
{
	if (isContainerPosChange || gD3D->GetResized())
	{
		mUiSet.background->SetPosition(mPosition.x, mPosition.y);
		mUiSet.outline->SetPosition(mPosition.x, mPosition.y);
		SetPosZ(mPosition.z);
		ClearPositionChangeListener();
	}
}

void UIStackContainer::UpdateWordBox()
{
	if (!mUiSet.text)return;
	UpdateWordBoxSize();
	UpdateWordPosition();
}

void UIStackContainer::UpdateWordBoxSize()
{
	//if there need any word box size change
	float boxWidth = mUIContainerSize.x - mPadding[UIContainerConfig::LEFT] - mPadding[UIContainerConfig::RIGHT];
	if(isWordAdaptive)
	{
		if (boxWidth <= mMinBoxWidth)
		{
			mUiSet.text->SetFontRectWidth(mMinBoxWidth);
		}
		else if (boxWidth >= mMaxBoxWidth)
		{
			mUiSet.text->SetFontRectWidth(mMaxBoxWidth);
		}else
		{
			mUiSet.text->SetFontRectWidth(boxWidth);
		}
	}
}

void UIStackContainer::UpdateWordPosition()
{
	float anchorPosX = 0.0f;
	float anchorPosY = 0.0f;
	switch (mUiSet.text->GetAnchorAlign())
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
	mUiSet.text->SetAnchorPos(anchorPosX, anchorPosY);
}

void UIStackContainer::TriggerListener()
{
}
