#pragma once
#include <wrl/client.h>
#include <vector>
#include "D3DApp.h" 

using Microsoft::WRL::ComPtr;

namespace ShaderEnum
{
	enum  ShaderKind {
		Vertex = 0,
		Pixel = 1,
	};

}

class Shader
{
public:
	Shader(ShaderEnum::ShaderKind _shader);
	virtual ~Shader();

protected:

	ShaderEnum::ShaderKind mShader;
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

	//void SetTexture(UINT slot, Texture* _texture);
	std::vector<ComPtr<ID3D11Buffer>> GetBuffers() { return mBuffers; };

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

	VertexShader(ShaderEnum::ShaderKind _shader);
	~VertexShader();

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
	PixelShader(ShaderEnum::ShaderKind _shader);
	~PixelShader() override;

	/// @brief Pixel Shader�쐬
	/// @param pData �ǂݍ���data
	/// @param size 
	/// @return 
	HRESULT CreateShader(void* pData, UINT size) override;

	/// @brief Set Constant Buffer to Pixel Shader
	void SetShader() override;


};