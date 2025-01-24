#pragma once
#include <DirectXMath.h>
#include <random>

class RandomGenerator
{
private:
	RandomGenerator() {};
	~RandomGenerator() = default;
	std::default_random_engine engine;  // ������������
public:
	static RandomGenerator* Get() { static RandomGenerator instance; return &instance; };
	void Init();

	float RandomFloat(float min = 0.0f, float max = 1.0f);
	int RandomInt(int min, int max);

	/// @brief �����������_���Ő�������
	/// @param thetaRange //XZ���ʂ̊p�x
	/// @param phiRange //XY���ʂ̊p�x
	/// @return 
	DirectX::XMFLOAT3 RandomDirection(float thetaRange,float phiRange);

	/// @brief �~�`�̒��Ń����_���̈ʒu����
	/// @param radius �~�̔��a
	///	@param center �~�S
	/// @return 
	DirectX::XMFLOAT3 RandomPointOnCircle(float radius, const DirectX::XMFLOAT3& center);

	DirectX::XMFLOAT4 RandomColor(const DirectX::XMFLOAT4& color, float colorRange);

	DirectX::XMFLOAT4 RandomColor();

	/// @brief 
	/// @param valueRange ���x�͈�(0~1)
	/// @param saturationRange �ʓx�͈�(0~1)
	/// @param colorRange �F�͈�(0~360)
	/// @return 
	DirectX::XMFLOAT4 RandomColor(const DirectX::XMFLOAT2& valueRange, const DirectX::XMFLOAT2& saturationRange, const DirectX::XMFLOAT2& colorRange);
};

