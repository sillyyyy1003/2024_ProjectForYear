#include "RandomGenerator.h"
#include <ctime>
#include <SimpleMath.h>

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
	
	float theta = thetaRange * XM_PI / 180.f;
	float phi=phiRange * XM_PI / 180.f;
	theta *= RandomFloat();
	phi *= RandomFloat();

	Vector3 direction = { cos(phi) * sin(theta) ,sin(phi),cos(phi) * cos(theta) };
	direction.Normalize();

	return direction;
}

