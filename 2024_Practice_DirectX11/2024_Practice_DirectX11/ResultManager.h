#pragma once
#include <memory>

#include "D2D_UIStackContainer.h"
#include "MissionManager.h"

class ResultManager
{
private:
	//���ʂ̕`��
	std::unique_ptr<D2D_UIStackContainer> mResult;
	//���ʕ\�����邩�H
	bool isActive = false;

	bool isGenerateResult = false;

	std::string mResult;

	ResultManager() {};
	~ResultManager() = default;

public:
	static ResultManager* Get() { static ResultManager instance; return &instance; };

	/// @brief �f�[�^������
	void Init();

	void Update(float dt);

	void Draw();

	void ResultGenerator();

	void GenerateCurrentResult();

};

