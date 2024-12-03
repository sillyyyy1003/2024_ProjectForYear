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

	/// @brief 基本シェーダーに対応
	/// @return 
	HRESULT CompileShader(const char* pCode);

	/// @brief hlslファイルをロードそして作成
	/// @param _fileName
	///	オブジェクトに使う
	HRESULT LoadShaderFile(const char* _fileName);

	virtual void SetShader() = 0;

	/// @brief データをシェーダに読み込む
	/// @param slot 
	/// @param pData 更新するデータのポインター
	void WriteShader(UINT slot, void* pData);

	/// @brief テクスチャをつける
	/// @param slot 
	/// @param _texture 
	void SetTexture(UINT slot, Texture* _texture);

	/*std::vector<ComPtr<ID3D11Buffer>> GetBuffers() { int bufferIndex = GameApp::GetRenderIndex(); return mBuffers[bufferIndex]; };*/

protected:
	/// @brief シェーダを作成
	/// @param pData 
	/// @param size 
	/// @return 
	virtual HRESULT CreateShader(void* pData, UINT size) = 0;


private:
	/// @brief 定数バッファとシェーダーを作成
	/// @param pData 
	/// @param size 
	/// @return 
	HRESULT Create(void* pData, UINT size);


};

/// @brief InputLayout & VertexShader 処理
class VertexShader :public Shader
{

private:
	ComPtr<ID3D11InputLayout> mInputLayout;
	ComPtr<ID3D11VertexShader> mVertexShader;

public:

	VertexShader();
	~VertexShader() override;

	/// @brief VertexShader作成
	/// @param pData 読み込んたdata
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

	/// @brief Pixel Shader作成
	/// @param pData 読み込んたdata
	/// @param size 
	/// @return 
	HRESULT CreateShader(void* pData, UINT size) override;

	/// @brief Set Constant Buffer to Pixel Shader
	void SetShader() override;



};