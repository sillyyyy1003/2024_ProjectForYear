#pragma once
#include <DirectXMath.h>
#include <random>

class RandomGenerator
{
private:
	RandomGenerator() {};
	~RandomGenerator() = default;
	std::default_random_engine engine;  // Š÷”¶¬Ší
public:
	static RandomGenerator* Get() { static RandomGenerator instance; return &instance; };
	void Init();

	float RandomFloat(float min = 0.0f, float max = 1.0f);
	int RandomInt(int min, int max);

	/// @brief 
	/// @param thetaRange 
	/// @param phiRange 
	/// @return 
	DirectX::XMFLOAT3 RandomDirection(float thetaRange,float phiRange);
};

