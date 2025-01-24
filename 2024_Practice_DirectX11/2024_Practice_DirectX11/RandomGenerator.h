#pragma once
#include <DirectXMath.h>
#include <random>

class RandomGenerator
{
private:
	RandomGenerator() {};
	~RandomGenerator() = default;
	std::default_random_engine engine;  // 随机数生成器
public:
	static RandomGenerator* Get() { static RandomGenerator instance; return &instance; };
	void Init();

	float RandomFloat(float min = 0.0f, float max = 1.0f);
	int RandomInt(int min, int max);

	/// @brief 方向をランダムで生成する
	/// @param thetaRange //XZ平面の角度
	/// @param phiRange //XY平面の角度
	/// @return 
	DirectX::XMFLOAT3 RandomDirection(float thetaRange,float phiRange);

	/// @brief 円形の中でランダムの位置生成
	/// @param radius 円の半径
	///	@param center 円心
	/// @return 
	DirectX::XMFLOAT3 RandomPointOnCircle(float radius, const DirectX::XMFLOAT3& center);

	DirectX::XMFLOAT4 RandomColor(const DirectX::XMFLOAT4& color, float colorRange);

	DirectX::XMFLOAT4 RandomColor();

	/// @brief 
	/// @param valueRange 明度範囲(0~1)
	/// @param saturationRange 彩度範囲(0~1)
	/// @param colorRange 色範囲(0~360)
	/// @return 
	DirectX::XMFLOAT4 RandomColor(const DirectX::XMFLOAT2& valueRange, const DirectX::XMFLOAT2& saturationRange, const DirectX::XMFLOAT2& colorRange);
};

