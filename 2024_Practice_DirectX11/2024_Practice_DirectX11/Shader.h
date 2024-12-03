#pragma once
#include <wrl/client.h>
#include <vector>
#include "SceneBase.h"
#include "Texture.h"

using Microsoft::WRL::ComPtr;


class Shader :public Component
{
protected:
	enum  ShaderKind {
		Vertex = 0,
		Pixel = 1,
	};
public:
	Shader(ShaderKind kind);
	virtual ~Shader();

protected:

	ShaderKind mShader;
	
	std::vector<ComPtr<ID3D11Buffer>> mBuffers;
	std::vector<ComPtr<ID3D11ShaderResourceView>> mTextures;

public:

	/// @brief ��{�V�F�[�_�[�ɑΉ�
	/// @return 
	HRESULT CompileShader(const char* pCode);

	/// @brief hlsl�t�@�C�������[�h�����č쐬
	/// @param _fileName
	///	�I�u�W�F�N�g�Ɏg��
	HRESULT LoadShaderFile(const char* _fileName);

	virtual void SetShader() = 0;

	/// @brief �f�[�^���V�F�[�_�ɓǂݍ���
	/// @param slot 
	/// @param pData �X�V����f�[�^�̃|�C���^�[
	void WriteShader(UINT slot, void* pData);

	/// @brief �e�N�X�`��������
	/// @param slot 
	/// @param _texture 
	void SetTexture(UINT slot, Texture* _texture);

	/*std::vector<ComPtr<ID3D11Buffer>> GetBuffers() { int bufferIndex = GameApp::GetRenderIndex(); return mBuffers[bufferIndex]; };*/

protected:
	/// @brief �V�F�[�_���쐬
	/// @param pData 
	/// @param size 
	/// @return 
	virtual HRESULT CreateShader(void* pData, UINT size) = 0;


private:
	/// @brief �萔�o�b�t�@�ƃV�F�[�_�[���쐬
	/// @param pData 
	/// @param size 
	/// @return 
	HRESULT Create(void* pData, UINT size);


};

/// @brief InputLayout & VertexShader ����
class VertexShader :public Shader
{

private:
	ComPtr<ID3D11InputLayout> mInputLayout;
	ComPtr<ID3D11VertexShader> mVertexShader;

public:

	VertexShader();
	~VertexShader() override;

	/// @brief VertexShader�쐬
	/// @param pData �ǂݍ���data
	/// @param size 
	/// @return 
	HRESULT CreateShader(void* pData, UINT size) override;

	/// @brief Set Constant Buffer to VtxPosColorNormal Shader
	void SetShader() override;
	ComPtr<ID3D11InputLayout> GetInputLayOut() { return mInputLayout; };
};

class PixelShader :public Shader
{

private:
	ComPtr<ID3D11PixelShader> mPixelShader;

public:
	PixelShader();
	~PixelShader() override;

	/// @brief Pixel Shader�쐬
	/// @param pData �ǂݍ���data
	/// @param size 
	/// @return 
	HRESULT CreateShader(void* pData, UINT size) override;

	/// @brief Set Constant Buffer to Pixel Shader
	void SetShader() override;



};