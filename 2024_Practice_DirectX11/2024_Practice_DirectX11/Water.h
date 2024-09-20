#pragma once
#include "Circle.h"
#include "Plane3D.h"


class Water
{


	struct WaterParam
	{
		DirectX::XMFLOAT3  center;  // �g�̒��S�ʒu
		float amplitude;			// �g�̕�
		float frequency;			// �g�̕p�x
		float speed;				// �g�̃X�r�[�h
		float sigma;				// �g�̊g�U�͈�
		float time;					// �o�ߎ���
	};

	WaterParam mParam = {};
	//std::unique_ptr<Plane3D> mModel;

	std::unique_ptr<Circle> mModel;
	//�g��J�n���ǂ����H
	bool isTrigger = true;
	float duration = 3.0f;


public:
	
	Water();
	~Water() = default;

	void Init(const char* filePath);
	void Update(float dt);
	void Draw();

	/// @brief WaterParam�̓��e���V�F�[�_�ɏ�������
	void WriteShader();

	//Plane3D* GetModel() { return mModel.get(); };
	Circle* GetModel() { return mModel.get(); };
};

