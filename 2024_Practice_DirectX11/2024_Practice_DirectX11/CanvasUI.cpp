#include "CanvasUI.h"
#include "Texture.h"
using namespace DirectX::SimpleMath;

void CanvasUI::InitCanvas(const char* _fileName)
{
	CreateMeshBuffer();
	CreateMaterial(_fileName);
	
}

void CanvasUI::CreateMeshBuffer()
{
}

void CanvasUI::CreateTexture(const char* _fileName)
{
	mMaterial.tex = std::make_shared<Texture>();
	mMaterial.tex->Create(_fileName);
}

void CanvasUI::CreateMaterial(const char* _fileName)
{
	mMaterial.material = {
		Color(1.0f, 1.0f, 1.0, 1.0f),		// ����
		Color(1.0f, 1.0f, 1.0, 1.0f),		// �\�ʐF
		Color(1.0f, 0.5f, 0.5f, 0.2f),		// ���ʔ���: specular power 1
		Color(0.0f, 0.0f, 0.0f, 0.0f)		// �������Ȃ�};
	};
	CreateTexture(_fileName);
}
