#include "RandomGenerator.h"
#include <ctime>
#include <SimpleMath.h>

#include "D3DUtil.h"

using namespace DirectX;
using namespace  DirectX::SimpleMath;

void RandomGenerator::Init()
{
	engine.seed(static_cast<unsigned int>(std::time(0)));
}

float RandomGenerator::RandomFloat(float min, float max)
{
	std::uniform_real_distribution<float> distribution(min, max);
	return distribution(engine);
}

int RandomGenerator::RandomInt(int min, int max)
{
	std::uniform_int_distribution<int> distribution(min, max);
	return distribution(engine);
}

DirectX::XMFLOAT3 RandomGenerator::RandomDirection(float thetaRange, float phiRange)
{
	
	float theta = thetaRange * XM_PI / 180.f;	//XZ平面の角度
	float phi=phiRange * XM_PI / 180.f;			//XY平面の角度

	theta *= RandomFloat();
	phi *= RandomFloat();

	Vector3 direction = { cos(phi) * sin(theta) ,sin(phi),cos(phi) * cos(theta) };
	direction.Normalize();

	return direction;
}

DirectX::XMFLOAT3 RandomGenerator::RandomPointOnCircle(float radius, const DirectX::XMFLOAT3& center)
{

	float theta = RandomFloat() * XM_2PI;

	//角度ランダム、半径ランダム
	Vector3 position = {
		sin(theta) * radius * RandomFloat() + center.x,	
		center.y,
		cos(theta) * radius * RandomFloat() + center.z };
	return position;
}

DirectX::XMFLOAT4 RandomGenerator::RandomColor(const DirectX::XMFLOAT4& color, float colorRange)
{
	HSV hsv = RGBtoHSV(color);
	hsv.hue += RandomFloat(-1.f, 1.f) * colorRange;

	return HSVtoRGB(hsv);
}

DirectX::XMFLOAT4 RandomGenerator::RandomColor()
{
	float h, s, v;
	h = 360.f * RandomFloat();

	//明度と彩度を暗くならないように制限する
	s = RandomFloat(0.7f,1.0f);
	v = RandomFloat(0.7f,1.0f);

	Color color = HSVtoRGB({ h,s,v,1.0f });
	return color;
}

DirectX::XMFLOAT4 RandomGenerator::RandomColor(const DirectX::XMFLOAT2& valueRange, const DirectX::XMFLOAT2& saturationRange, const DirectX::XMFLOAT2& colorRange)
{
	float h, s, v;
	h = RandomFloat(colorRange.x, colorRange.y);
	s = RandomFloat(saturationRange.x, saturationRange.y);
	v = RandomFloat(valueRange.x, valueRange.y);

	return HSVtoRGB({ h,s,v,1.0f });
}


