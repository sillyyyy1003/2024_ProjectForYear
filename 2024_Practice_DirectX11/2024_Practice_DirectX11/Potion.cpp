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

		PotionColor::HSV base = RGBtoHSV(baseColor);
		PotionColor::HSV mix = RGBtoHSV(color);
		PotionColor::HSV outputColor = BlendColor(base, mix, alpha);
		result = HSVtoColor(outputColor);
		result.A(baseColor.w);
	
	}

	mModel->SetDiffuse(result);

}

PotionColor::HSV Potion::RGBtoHSV(const DirectX::XMFLOAT4& color)
{
	float maxVal = std::max({ color.x,color.y,color.z });
	float minVal = std::min({ color.x,color.y,color.z });

	float delta = maxVal - minVal;
	float h = 0.f, s = 0.f, v = maxVal;

	if (delta > 0)
	{
		if(maxVal==color.x)
		{
			h = 60.f * (fmod(((color.y - color.z) / delta), 6.0f));
		}else if(maxVal==color.y)
		{
			h = 60.f *(((color.z - color.x) / delta) + 2.0f);
		}else
		{
			h = 60.f * (((color.x - color.y) / delta) + 4.f);
		}

		if (maxVal > 0)
		{
			s = delta / maxVal;
		}
	}
	if (h < 0.f)h += 360.f;

	return { h,s,v };

}

PotionColor::HSV Potion::BlendColor(const PotionColor::HSV& color1, const PotionColor::HSV& color2, float alpha)
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

DirectX::XMFLOAT4 Potion::HSVtoColor(const PotionColor::HSV& color)
{
	float c = color.value * color.saturation;
	float x = c * (1.f - fabs(fmod(color.hue / 60.f, 2.f) - 1.f));
	float m = color.value - c;

	float r, g, b = 0.0f;
	Color outputColor;
	if (color.hue >= 0 && color.hue < 60.f)
	{
		r = c;
		g = x;
		b = 0;
	}
	else if (color.hue >= 60.f && color.hue < 120.f)
	{
		r = x;
		g = c;
		b = 0;
	}
	else if (color.hue >= 120.f && color.hue < 180.f)
	{
		r = 0;
		g = c;
		b = x;
	}
	else if (color.hue >= 180.f && color.hue < 240.f)
	{
		r = 0;
		g = x;
		b = c;
	}
	else if (color.hue >= 240.f && color.hue < 300.f)
	{
		r = x;
		g = 0;
		b = c;

	}else
	{
		r = c;
		g = 0;
		b = x;
	}

	outputColor.R(r + m);
	outputColor.G(g + m);
	outputColor.B(b + m);
	outputColor.A(1.0f);
	return outputColor;
}

void Potion::AutoColor(float dt)
{
	Color color = mModel->GetMaterial().diffuse;
	PotionColor::HSV currentColor = RGBtoHSV(color);
	currentColor.hue += mColorTransformSpeed * dt;
	currentColor.hue = fmod(currentColor.hue,360.f);

	Color result = HSVtoColor(currentColor);
	result.A(color.w);
	mModel->SetDiffuse(result);

}

void Potion::SetWaterColor(DirectX::XMFLOAT4 color)
{
	mModel->SetDiffuse(color);
}
