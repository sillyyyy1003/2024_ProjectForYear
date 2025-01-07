#include "Potion.h"

void Potion::Update(float dt)
{
	//Set Wave Center Pos
	Water::Update(dt);

	if (isAutoColor)
		AutoColor(dt);


}

void Potion::Dilute()
{
	Color baseColor = this->mModel->GetMaterial().diffuse;
	Color resultColor = baseColor;
	resultColor.R((1.f - mDiluteParam) * baseColor.x + mDiluteParam);
	resultColor.G((1.f - mDiluteParam) * baseColor.y + mDiluteParam);
	resultColor.B((1.f - mDiluteParam) * baseColor.z + mDiluteParam);

	mModel->SetDiffuse(resultColor);
}

void Potion::Dilute(float diluteAlpha)
{
	float param = diluteAlpha * mDiluteParam * 0.05f;
	Color baseColor = this->mModel->GetMaterial().diffuse;
	Color resultColor = baseColor;
	resultColor.R((1.f - param) * baseColor.x + param);
	resultColor.G((1.f - param) * baseColor.y + param);
	resultColor.B((1.f - param) * baseColor.z + param);

	mModel->SetDiffuse(resultColor);
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

	}else
	{

		HSV base = RGBtoHSV(baseColor);
		HSV mix = RGBtoHSV(color);
		HSV outputColor = BlendColor(base, mix, alpha);
		result = HSVtoRGB(outputColor);
		result.A(baseColor.w);
	
	}

	mModel->SetDiffuse(result);

}


HSV Potion::BlendColor(const HSV& color1, const HSV& color2, float alpha)
{
	float h, s, v;

	//(0,30)
	
	if (alpha > 1)
	{
		h = color2.hue;
	}else
	{
		// calculate delta hue;
		float dh = fmod(color2.hue - color1.hue, 360.f);
		if (dh > 0)
		{
			if (dh > 180.f)
			{
				h = fmod(color1.hue - alpha * dh + 360.f, 360.f);
			}
			else
			{
				h = color1.hue + alpha * dh;
			}
		}
		else
		{
			if (dh < -180.f)
			{
				h = fmod(color1.hue - alpha * dh, 360.f);
			}else
			{
				h = color1.hue + alpha * dh;
			}
		}

	}

	s = color1.saturation + alpha * (color2.saturation - color1.saturation);
	v = color1.value + alpha * (color2.value - color1.value);
	return{ h,s,v };
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
