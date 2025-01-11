#include "Potion.h"

void Potion::Update(float dt)
{
	//Set Wave Center Pos
	Water::Update(dt);
#ifdef _DEBUG
	if (ImGui::Begin("PotionParam"))
	{
		ImGui::InputFloat("defaultHeight", &mDefaultHeight);

		ImGui::InputFloat("DiluteParam", &mDiluteParam);

		ImGui::InputFloat("ColorTransformSpeed", &mColorTransformSpeed);

		ImGui::InputFloat("ValueParam", &mValue);
	}
	ImGui::End();
#endif
	//色の自動変換
	if (isAutoColor)
		AutoColor(dt);


}

void Potion::Dilute(float diluteAlpha)
{
	float param = diluteAlpha * mDiluteParam;
	HSV baseColor = RGBtoHSV(this->mModel->GetMaterial().diffuse);
	if (baseColor.saturation >= 0.25f)
	{
		baseColor.saturation -= param;
		baseColor.alpha -= param;
		mModel->SetDiffuse(HSVtoRGB(baseColor));
	}

	RiseUpWaterLevel(param);
}
void Potion::MixColor(DirectX::XMFLOAT4 color, float alpha)
{
	Color baseColor = this->mModel->GetMaterial().diffuse;
	baseColor.w += alpha * 0.5f;
	
	Color result;
	if (baseColor.x == baseColor.y && baseColor.x == baseColor.z)//if is white color
	{
		float param = alpha * mDiluteParam ;
		result.R((1.f - param) * color.x + param * color.x);
		result.G((1.f - param) * color.x + param * color.y);
		result.B((1.f - param) * color.x + param * color.z);
		result.A(baseColor.w);
		mPreviousHue = RGBtoHSV(color).hue;
	}else
	{
		HSV outputColor = BlendColor(RGBtoHSV(baseColor), RGBtoHSV(color), alpha);
		result = HSVtoRGB(outputColor);
	}

	//現在の色を設定する
	mModel->SetDiffuse(result);

	float param = alpha;
	RiseUpWaterLevel(param);
}


HSV Potion::BlendColor(const HSV& baseColor, const HSV& mixColor, float alpha)
{
	float h, s, v, a = 0.f;

	
	/*
	if (alpha > 1)
	{
		h = mixColor.hue;
	}else
	{
		// calculate delta hue;
		float dh = fmod(mixColor.hue - baseColor.hue, 360.f);
		if (dh > 0)
		{
			if (dh > 180.f)
			{
				h = fmod(baseColor.hue - alpha * dh + 360.f, 360.f);
			}
			else
			{
				h = baseColor.hue + alpha * dh;
			}
		}
		else
		{
			if (dh < -180.f)
			{
				h = fmod(baseColor.hue - alpha * dh, 360.f);
			}else
			{
				h = baseColor.hue + alpha * dh;
			}
		}
	}
	*/

	if (baseColor.hue < 15)//Red～OrangeArea
	{
#ifdef _DEBUG
		DebugLog::Log("The Color is Red～OrangeArea");
#endif

		if (mixColor.hue == 0.0f)
		{
			//BaseColorHue>MixColorHue
			float deltaHue = baseColor.hue - mixColor.hue;
			h = baseColor.hue - deltaHue * alpha;
		}
		else if (mixColor.hue == 60.0f)
		{
			float deltaHue = mixColor.hue - baseColor.hue;
			h = baseColor.hue + deltaHue * alpha;
		}
		else  if (mixColor.hue == 240.0f)
		{
			float deltaHue = baseColor.hue + 360.f - mixColor.hue;
			h = baseColor.hue - deltaHue * alpha;

			if (h <= 0)//赤に超えたら、紫に移動する
				h += 360.f;
		}

	}
	else if (baseColor.hue >= 15.f && baseColor.hue < 60.f)//Orange~Yellow
	{
#ifdef _DEBUG
		DebugLog::Log("The Color is Orange~Yellow");
#endif

		if (mixColor.hue == 0.0f)//赤
		{	//BaseColorHue>MixColorHue
			float deltaHue = baseColor.hue - mixColor.hue;
			h = fmod(baseColor.hue - deltaHue * alpha, 360.f);
		}else if(mixColor.hue == 60.0f)//Yellow
		{
			float deltaHue = mixColor.hue - baseColor.hue;
			h = fmod(baseColor.hue + deltaHue * alpha, 360.f);
		}else//Blue
		{
			float deltaHue = mixColor.hue-baseColor.hue;
			h = fmod(baseColor.hue + deltaHue * alpha, 360.f);
		}
		
	}
	else if (baseColor.hue >= 60.f && baseColor.hue < 180.f)//Yellow~Green
	{
#ifdef _DEBUG
		DebugLog::Log("The Color is Yellow~Green");
#endif

		if (mixColor.hue == 0.0f)//赤
		{	//BaseColorHue>MixColorHue
			float deltaHue = baseColor.hue - mixColor.hue;
			h = fmod(baseColor.hue - deltaHue * alpha, 360.f);
		}
		else if (mixColor.hue == 60.0f)//Yellow
		{
			float deltaHue = mixColor.hue - baseColor.hue;
			h = fmod(baseColor.hue + deltaHue * alpha, 360.f);
		}
		else//Blue
		{
			float deltaHue = mixColor.hue - baseColor.hue;
			h = fmod(baseColor.hue + deltaHue * alpha, 360.f);
		}
	}
	else if (baseColor.hue >= 180.f && baseColor.hue < 240.f)//GreenBlue~Blue
	{
#ifdef _DEBUG
		DebugLog::Log("The Color is GreenBlue~Blue");
#endif

		if (mixColor.hue == 0.0f)//赤
		{
			float deltaHue = 360.f - baseColor.hue;
			h = fmod(baseColor.hue + deltaHue * alpha, 360.f);
		}
		else if (mixColor.hue == 60.0f)//Yellow
		{
			float deltaHue = baseColor.hue-mixColor.hue;
			h = fmod(baseColor.hue - deltaHue * alpha, 360.f);
		}
		else//Blue
		{
			float deltaHue = mixColor.hue - baseColor.hue;
			h = fmod(baseColor.hue + deltaHue * alpha, 360.f);
		}
	}else if(baseColor.hue >= 240.f && baseColor.hue <360.f)//Blue~Purple
	{
#ifdef _DEBUG
		DebugLog::Log("The Color is Blue~Purple");
#endif
		if(mixColor.hue == 0.0f)//赤
		{
			float deltaHue = 360.f - baseColor.hue;
			h = fmod(baseColor.hue + deltaHue * alpha, 360.f);
		}
		else if (mixColor.hue == 60.0f)//Yellow
		{
			float deltaHue = baseColor.hue - mixColor.hue;
			h = fmod(baseColor.hue - deltaHue * alpha, 360.f);
			}
		else//Blue
		{
			float deltaHue = mixColor.hue - baseColor.hue;
			h = fmod(baseColor.hue + deltaHue * alpha, 360.f);
		}
	}


	//彩度計算
	s = baseColor.saturation + alpha * (mixColor.saturation - baseColor.saturation);

	//明度計算	
	v = baseColor.value + alpha * (mixColor.value - baseColor.value);
	if (mPreviousHue != mixColor.hue)
	{
		//明度が徐々に変化する
		accumulateValue += alpha;
		if (accumulateValue <= mValue)
		{
			v -= alpha;
		}
		
	}

	a = baseColor.alpha;

	//透明度計算
	if (a < 0.9f)
	{
		a = baseColor.alpha + (mixColor.alpha - baseColor.alpha) * alpha;
	}else
	{
		a = 0.9f;
	}

	return{ h,s,v ,a};
}

void Potion::AutoColor(float dt)
{
	Color color = mModel->GetMaterial().diffuse;
	HSV currentColor = RGBtoHSV(color);
	currentColor.hue += mColorTransformSpeed * dt;
	currentColor.hue = fmod(currentColor.hue,360.f);

	Color result = HSVtoRGB(currentColor);
	result.A(color.w);
	mModel->SetDiffuse(result);

}

void Potion::SetWaterColor(DirectX::XMFLOAT4 color)
{
	mModel->SetDiffuse(color);
}

void Potion::LoadSaveData(json data, const char* objName)
{
	Water::LoadSaveData(data, objName);
	mDefaultHeight = data[objName]["DefaultHeight"][0];
	mDiluteParam = data[objName]["DiluteParam"][0];
	mColorTransformSpeed = data[objName]["TransformSpeed"][0];
	mValue = data[objName]["ValueParam"][0];
	mMaxHeight = data[objName]["MaxHeight"][0];
	mLimitHeight = data[objName]["LimitHeight"][0];
	//sui
	InitWaterLevelParam();
	
}

json Potion::SaveData()
{
	json data;
	data = Water::SaveData();
	data["DefaultHeight"] = { mDefaultHeight };
	data["DiluteParam"] = { mDiluteParam };
	data["TransformSpeed"] = { mColorTransformSpeed };
	data["ValueParam"] = { mValue };
	data["MaxHeight"] = { mMaxHeight };
	data["LimitHeight"] = { mLimitHeight };
	return data;
}

void Potion::SetPreviousHue(float hue)
{
	mPreviousHue = hue;
	
	accumulateValue = 0.0f;	//累計係数リセット
}

void Potion::RiseUpWaterLevel(float param)
{
	if (mModel->GetPosition().y < mMaxHeight)
	{
		DirectX::XMFLOAT3 pos = mModel->GetPosition();
		pos.y += param;
		mModel->SetPosition(pos);

		//指定水位に超えたら、カメラ揺れが始まる
		if (mModel->GetPosition().y > mLimitHeight)
			isOverLimitHeight = true;
	}else
	{
		isMaxHeight = true;
	}

}

void Potion::InitWaterLevelParam()
{
	//初期水位を設定する
	DirectX::XMFLOAT3 pos = mModel->GetPosition();
	pos.y = mDefaultHeight;
	mModel->SetPosition(pos);
}

float Potion::CalculateCurrentShakingParam()
{
	//有効範囲内ではない場合
	if (mModel->GetPosition().y < mLimitHeight || mModel->GetPosition().y>mMaxHeight)
		return 0;

	//0~1の値を返す
	//todo:先使用线性变化，之后可该为指数映射
	return (mModel->GetPosition().y - mLimitHeight) / (mMaxHeight - mLimitHeight);
}

void Potion::ResetWaterLevel()
{
	//初期水位を設定する
	DirectX::XMFLOAT3 pos = mModel->GetPosition();
	pos.y = mDefaultHeight;
	mModel->SetPosition(pos);
}
