#pragma once
#include <DirectXMath.h>
#include <random>

class RandomGenerator
{
private:
	RandomGenerator() {};
	~RandomGenerator() = default;
	std::default_random_engine engine;  // ÷śŹí
public:
	static RandomGenerator* Get() { static RandomGenerator instance; return &instance; };
	void Init();

	float RandomFloat(float min = 0.0f, float max = 1.0f);
	int RandomInt(int min, int max);

	/// @brief űüđ_ĹśŹˇé
	/// @param thetaRange //XZ˝ĘĚpx
	/// @param phiRange //XY˝ĘĚpx
	/// @return 
	DirectX::XMFLOAT3 RandomDirection(float thetaRange,float phiRange);

	/// @brief ~`ĚĹ_ĚĘuśŹ
	/// @param radius ~Ěźa
	///	@param center ~S
	/// @return 
	DirectX::XMFLOAT3 RandomPointOnCircle(float radius, const DirectX::XMFLOAT3& center);

	DirectX::XMFLOAT4 RandomColor(const DirectX::XMFLOAT4& color, float colorRange);

	DirectX::XMFLOAT4 RandomColor();

	/// @brief 
	/// @param valueRange žxÍÍ(0~1)
	/// @param saturationRange ĘxÍÍ(0~1)
	/// @param colorRange FÍÍ(0~360)
	/// @return 
	DirectX::XMFLOAT4 RandomColor(const DirectX::XMFLOAT2& valueRange, const DirectX::XMFLOAT2& saturationRange, const DirectX::XMFLOAT2& colorRange);
};

