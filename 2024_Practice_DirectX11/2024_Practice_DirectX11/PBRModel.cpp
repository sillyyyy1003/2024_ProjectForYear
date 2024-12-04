#include "PBRModel.h"

#include "Model.h"
#include <memory>
#include "DebugLog.h"
#include "Shader.h"
#include <assimp/postprocess.h>

#include "DirLight.h"
#include "FirstPersonCamera.h"
#include "GameApp.h"

#ifdef _DEBUG
#pragma comment(lib, "assimp-vc143-mtd.lib")
#else
#pragma comment(lib, "assimp-vc143-mt.lib")
#endif

using namespace DirectX;
using namespace DirectX::SimpleMath;

PBRModel::PBRModel() :Primitive(PrimitiveConfig::MULTI)
{
	importer = std::make_unique<Assimp::Importer>();
}

PBRModel::~PBRModel()
{

}

void PBRModel::Init(const char* filePath)
{
	Load(filePath);
	//LoadDefShader();

}

void PBRModel::InitWithoutTex(const char* file)
{
	LoadWithoutTex(file);
	//LoadDefShader();

}

bool PBRModel::Load(const char* file, bool flip, bool simpleMode)
{
	DebugLog::Log("モデル読み込み開始");

	int flag = 0;
	if (simpleMode)
	{
		flag |= aiProcess_Triangulate;					// 非三角ポリゴンを三角に割る
		flag |= aiProcess_JoinIdenticalVertices;		// 同一位置頂点を一つに統合する
		flag |= aiProcess_FlipUVs;						//　UV値をY軸を基準に反転させる
		flag |= aiProcess_PreTransformVertices;			// ノードを一つに統合 !!アニメーション情報が消えることに注意!!
		if (flip) flag |= aiProcess_MakeLeftHanded;		// 左手系座標に変換
	}
	else
	{
		flag |= aiProcessPreset_TargetRealtime_MaxQuality;	// リアルタイム レンダリング用にデータを最適化するデフォルトの後処理構成。
		flag |= aiProcess_PopulateArmatureData;				// 標準的なボーン,アーマチュアの設定
		if (flip) flag |= aiProcess_ConvertToLeftHanded;	// 左手系変更オプションがまとまったもの
	}

	// assimpで読み込み
	mScene = importer.get()->ReadFile(file, flag);
	if (!mScene)
	{
		Error(importer->GetErrorString());
		DebugLog::Log("{} Assimpモデルロード失敗", file);
		return false;
	}

	// メッシュの作成
	aiVector3D zero(0.0f, 0.0f, 0.0f);
	std::vector<Vertex::VtxPosNormalTex> vtxGroup;
	for (unsigned int i = 0; i < mScene->mNumMeshes; ++i)
	{
		MeshBuffer mesh = {};

		// 頂点の作成
		std::vector<Vertex::VtxPosNormalTangentTex> vtx;
		vtx.resize(mScene->mMeshes[i]->mNumVertices);
		for (unsigned int j = 0; j < vtx.size(); ++j)
		{
			// 値の吸出し
			aiVector3D pos = mScene->mMeshes[i]->mVertices[j];
			aiVector3D uv = mScene->mMeshes[i]->HasTextureCoords(0) ?
				mScene->mMeshes[i]->mTextureCoords[0][j] : zero;
			aiVector3D normal = mScene->mMeshes[i]->HasNormals() ?
				mScene->mMeshes[i]->mNormals[j] : zero;
			aiVector3D tangent = mScene->mMeshes[i]->HasTangentsAndBitangents() ?
				mScene->mMeshes[i]->mTangents[j] : zero;
			// 値を設定
			vtx[j] = {
				XMFLOAT3(pos.x, pos.y, pos.z),
				XMFLOAT3(normal.x, normal.y, normal.z),
				XMFLOAT2(uv.x, 1-uv.y),
				XMFLOAT3(tangent.x,tangent.y,tangent.z),
			};
		
			vtxGroup.push_back({
				XMFLOAT3(pos.x, pos.y, pos.z),
				XMFLOAT3(normal.x, normal.y, normal.z),
				XMFLOAT2(uv.x, 1 - uv.y),
				});
		}
	
	

		// インデックスの作成
		std::vector<unsigned int> idx;
		idx.resize(mScene->mMeshes[i]->mNumFaces * 3); // faceはポリゴンの数を表す(１ポリゴンで3インデックス
		for (unsigned int j = 0; j < mScene->mMeshes[i]->mNumFaces; ++j)
		{
			aiFace face = mScene->mMeshes[i]->mFaces[j];
			int faceIdx = j * 3;
			idx[faceIdx + 0] = face.mIndices[0];
			idx[faceIdx + 1] = face.mIndices[1];
			idx[faceIdx + 2] = face.mIndices[2];
		}

		// マテリアルの割り当て
		mesh.materialID = mScene->mMeshes[i]->mMaterialIndex;

		// メッシュを元に頂点バッファ作成
		Mesh::MeshData desc = {};
		desc.pVertex = vtx.data();
		desc.vertexSize = sizeof(Vertex::VtxPosNormalTangentTex);
		desc.vertexCount = static_cast<UINT>(vtx.size());
		desc.pIndex = idx.data();
		desc.indexSize = sizeof(unsigned int);
		desc.indexCount = static_cast<UINT>(idx.size());
		desc.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		mesh.mesh = std::make_shared<Mesh>(desc);

		// メッシュ追加
		mMeshes.push_back(mesh);
	}

	this->SetVertices(vtxGroup);

	//--- マテリアルの作成
	// ファイルの探索
	std::string dir = file;
	dir = dir.substr(0, dir.find_last_of('/') + 1);
	// マテリアル
	aiColor3D color(0.0f, 0.0f, 0.0f);
	DirectX::XMFLOAT4 diffuse(1.0f, 1.0f, 1.0f, 1.0f);
	DirectX::XMFLOAT4 ambient(0.3f, 0.3f, 0.3f, 1.0f);

	for (unsigned int i = 0; i < mScene->mNumMaterials; ++i)
	{

		PBRMaterial material = {};

		// 各種パラメーター
		float shininess;
		material.material.diffuse = mScene->mMaterials[i]->Get(AI_MATKEY_COLOR_DIFFUSE, color) == AI_SUCCESS ?
			DirectX::XMFLOAT4(color.r, color.g, color.b, 1.0f) : diffuse;
		material.material.ambient = mScene->mMaterials[i]->Get(AI_MATKEY_COLOR_AMBIENT, color) == AI_SUCCESS ?
			DirectX::XMFLOAT4(color.r, color.g, color.b, 1.0f) : ambient;
		shininess = mScene->mMaterials[i]->Get(AI_MATKEY_SHININESS, shininess) == AI_SUCCESS ? shininess : 0.0f;
		material.material.specular = mScene->mMaterials[i]->Get(AI_MATKEY_COLOR_SPECULAR, color) == AI_SUCCESS ?
			DirectX::XMFLOAT4(color.r, color.g, color.b, shininess) : DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, shininess);

		// テクスチャ
		aiString path;
		if (mScene->mMaterials[i]->Get(AI_MATKEY_TEXTURE_DIFFUSE(0), path) == AI_SUCCESS)
		{
			HRESULT hr;
			// ファイル形式チェック
			if (strstr(path.C_Str(), ".psd"))
			{
				DebugLog::LogError("モデルのテクスチャにpsdファイルが指定されています。psd読み込みには非対応。");
				Error("モデルのテクスチャにpsdファイルが指定されています。psd読み込みには非対応。");
			}
			// そのまま探索
			material.albedoTex = std::make_shared<Texture>();
			hr = material.albedoTex->Create(path.C_Str());
			// モデルと同じ階層を探索
			if (FAILED(hr))
			{
				std::string file = dir;
				file += path.C_Str();
				hr = material.albedoTex->Create(file.c_str());
			}
			// ファイル名のみで探索
			if (FAILED(hr))
			{
				std::string file = path.C_Str();
				if (size_t idx = file.find_last_of('\\'); idx != std::string::npos)
				{
					file = file.substr(idx + 1);
					file = dir + file;
					hr = material.albedoTex->Create(file.c_str());
				}

				// ファイル目のパスが"\\ではなく"/"の場合の対応
				if (FAILED(hr))
				{
					if (size_t idx = file.find_last_of('/'); idx != std::string::npos)
					{
						file = file.substr(idx + 1);
						file = dir + file;
						hr = material.albedoTex->Create(file.c_str());
					}
				}
			}
			// 失敗
			if (FAILED(hr))
			{
				DebugLog::LogWarning("Assimp: Filepath is wrong");
				Error(path.C_Str());
				material.albedoTex = nullptr;
				material.material.isTexEnable = false;
			}
		}
		else
		{
			DebugLog::LogWarning("FAILED to Load Albedo Material");
			material.albedoTex = nullptr;
			material.material.isTexEnable = false;
		}


		if (mScene->mMaterials[i]->Get(AI_MATKEY_TEXTURE_SHININESS(0), path) == AI_SUCCESS)
		{
			HRESULT hr;
			// ファイル形式チェック
			if (strstr(path.C_Str(), ".psd"))
			{
				DebugLog::LogError("モデルのテクスチャにpsdファイルが指定されています。psd読み込みには非対応。");
				Error("モデルのテクスチャにpsdファイルが指定されています。psd読み込みには非対応。");
			}
			// そのまま探索
			material.metallicTex = std::make_shared<Texture>();
			hr = material.metallicTex->Create(path.C_Str());
			// モデルと同じ階層を探索
			if (FAILED(hr))
			{
				std::string file = dir;
				file += path.C_Str();
				hr = material.metallicTex->Create(file.c_str());
			}
			// ファイル名のみで探索
			if (FAILED(hr))
			{
				std::string file = path.C_Str();
				if (size_t idx = file.find_last_of('\\'); idx != std::string::npos)
				{
					file = file.substr(idx + 1);
					file = dir + file;
					hr = material.metallicTex->Create(file.c_str());
				}

				// ファイル目のパスが"\\ではなく"/"の場合の対応
				if (FAILED(hr))
				{
					if (size_t idx = file.find_last_of('/'); idx != std::string::npos)
					{
						file = file.substr(idx + 1);
						file = dir + file;
						hr = material.metallicTex->Create(file.c_str());
					}
				}
			}
			// 失敗
			if (FAILED(hr))
			{
				DebugLog::LogWarning("Assimp: Filepath is wrong");
				Error(path.C_Str());
				material.metallicTex = nullptr;
				material.material.isTexEnable = false;
			}
		}
		else
		{
			DebugLog::LogWarning("FAILED to Load Metallic Material");
			material.metallicTex = nullptr;
		}

		if (mScene->mMaterials[i]->Get(AI_MATKEY_TEXTURE_HEIGHT(0), path) == AI_SUCCESS)
		{
			HRESULT hr;
			// ファイル形式チェック
			if (strstr(path.C_Str(), ".psd"))
			{
				DebugLog::LogError("モデルのテクスチャにpsdファイルが指定されています。psd読み込みには非対応。");
				Error("モデルのテクスチャにpsdファイルが指定されています。psd読み込みには非対応。");
			}
			// そのまま探索
			material.normalMap = std::make_shared<Texture>();
			hr = material.normalMap->Create(path.C_Str());
			// モデルと同じ階層を探索
			if (FAILED(hr))
			{
				std::string file = dir;
				file += path.C_Str();
				hr = material.normalMap->Create(file.c_str());
			}
			// ファイル名のみで探索
			if (FAILED(hr))
			{
				std::string file = path.C_Str();
				if (size_t idx = file.find_last_of('\\'); idx != std::string::npos)
				{
					file = file.substr(idx + 1);
					file = dir + file;
					hr = material.normalMap->Create(file.c_str());
				}

				// ファイル目のパスが"\\ではなく"/"の場合の対応
				if (FAILED(hr))
				{
					if (size_t idx = file.find_last_of('/'); idx != std::string::npos)
					{
						file = file.substr(idx + 1);
						file = dir + file;
						hr = material.normalMap->Create(file.c_str());
					}
				}
			}
			// 失敗
			if (FAILED(hr))
			{
				DebugLog::LogWarning("Assimp: Filepath is wrong");
				Error(path.C_Str());
				material.normalMap = nullptr;
			}
		}
		else
		{
			DebugLog::LogWarning("FAILED to Load Normal Map");
			material.normalMap = nullptr;
		}

		mMaterials.push_back(material);
	}

	DebugLog::Log("モデル読み込み完了");
	return true;
}

bool PBRModel::LoadWithoutTex(const char* file, bool flip, bool simpleMode)
{
	DebugLog::Log("モデル読み込み開始");

	int flag = 0;
	if (simpleMode)
	{
		flag |= aiProcess_Triangulate;					// 非三角ポリゴンを三角に割る
		flag |= aiProcess_JoinIdenticalVertices;		// 同一位置頂点を一つに統合する
		flag |= aiProcess_FlipUVs;						//　UV値をY軸を基準に反転させる
		flag |= aiProcess_PreTransformVertices;			// ノードを一つに統合 !!アニメーション情報が消えることに注意!!
		if (flip) flag |= aiProcess_MakeLeftHanded;		// 左手系座標に変換
	}
	else
	{
		flag |= aiProcessPreset_TargetRealtime_MaxQuality;	// リアルタイム レンダリング用にデータを最適化するデフォルトの後処理構成。
		flag |= aiProcess_PopulateArmatureData;				// 標準的なボーン,アーマチュアの設定
		if (flip) flag |= aiProcess_ConvertToLeftHanded;	// 左手系変更オプションがまとまったもの
	}

	// assimpで読み込み
	mScene = importer.get()->ReadFile(file, flag);
	if (!mScene)
	{
		Error(importer->GetErrorString());
		DebugLog::Log("{} Assimpモデルロード失敗", file);
		return false;
	}

	// メッシュの作成
	aiVector3D zero(0.0f, 0.0f, 0.0f);
	std::vector<Vertex::VtxPosNormalTex> vtxGroup;
	for (unsigned int i = 0; i < mScene->mNumMeshes; ++i)
	{
		MeshBuffer mesh = {};

		// 頂点の作成
		
		std::vector<Vertex::VtxPosNormalTangentTex> vtx;
		vtx.resize(mScene->mMeshes[i]->mNumVertices);
		for (unsigned int j = 0; j < vtx.size(); ++j)
		{
			// 値の吸出し
			aiVector3D pos = mScene->mMeshes[i]->mVertices[j];
			aiVector3D uv = mScene->mMeshes[i]->HasTextureCoords(0) ?
				mScene->mMeshes[i]->mTextureCoords[0][j] : zero;
			aiVector3D normal = mScene->mMeshes[i]->HasNormals() ?
				mScene->mMeshes[i]->mNormals[j] : zero;
			aiVector3D tangent = mScene->mMeshes[i]->HasTangentsAndBitangents() ?
				mScene->mMeshes[i]->mTangents[j] : zero;
			// 値を設定
			vtx[j] = {
				XMFLOAT3(pos.x, pos.y, pos.z),
				XMFLOAT3(normal.x, normal.y, normal.z),
				XMFLOAT2(uv.x, 1 - uv.y),
				XMFLOAT3(tangent.x,tangent.y,tangent.z),
			
			};
			vtxGroup.push_back(
				{
					XMFLOAT3(pos.x, pos.y, pos.z),
					XMFLOAT3(normal.x, normal.y, normal.z),
					XMFLOAT2(uv.x, 1 - uv.y),
				});
		}
	

		// インデックスの作成
		std::vector<unsigned int> idx;
		idx.resize(mScene->mMeshes[i]->mNumFaces * 3); // faceはポリゴンの数を表す(１ポリゴンで3インデックス
		for (unsigned int j = 0; j < mScene->mMeshes[i]->mNumFaces; ++j)
		{
			aiFace face = mScene->mMeshes[i]->mFaces[j];
			int faceIdx = j * 3;
			idx[faceIdx + 0] = face.mIndices[0];
			idx[faceIdx + 1] = face.mIndices[1];
			idx[faceIdx + 2] = face.mIndices[2];
		}

		// マテリアルの割り当て
		mesh.materialID = mScene->mMeshes[i]->mMaterialIndex;

		// メッシュを元に頂点バッファ作成
		Mesh::MeshData desc = {};
		desc.pVertex = vtx.data();
		desc.vertexSize = sizeof(Vertex::VtxPosNormalTangentTex);
		desc.vertexCount = static_cast<UINT>(vtx.size());
		desc.pIndex = idx.data();
		desc.indexSize = sizeof(unsigned int);
		desc.indexCount = static_cast<UINT>(idx.size());
		desc.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		mesh.mesh = std::make_shared<Mesh>(desc);

		// メッシュ追加
		mMeshes.push_back(mesh);
	}

	this->SetVertices(vtxGroup);
	//--- マテリアルの作成
	// ファイルの探索
	std::string dir = file;
	dir = dir.substr(0, dir.find_last_of('/') + 1);
	// マテリアル
	aiColor3D color(0.0f, 0.0f, 0.0f);
	DirectX::XMFLOAT4 diffuse(1.0f, 1.0f, 1.0f, 1.0f);
	DirectX::XMFLOAT4 ambient(0.3f, 0.3f, 0.3f, 1.0f);

	for (unsigned int i = 0; i < mScene->mNumMaterials; ++i)
	{
		PBRMaterial material = {};

		// 各種パラメーター
		float shininess;
		material.material.diffuse = mScene->mMaterials[i]->Get(AI_MATKEY_COLOR_DIFFUSE, color) == AI_SUCCESS ?
			DirectX::XMFLOAT4(color.r, color.g, color.b, 1.0f) : diffuse;
		material.material.ambient = mScene->mMaterials[i]->Get(AI_MATKEY_COLOR_AMBIENT, color) == AI_SUCCESS ?
			DirectX::XMFLOAT4(color.r, color.g, color.b, 1.0f) : ambient;
		shininess = mScene->mMaterials[i]->Get(AI_MATKEY_SHININESS, shininess) == AI_SUCCESS ? shininess : 0.0f;
		material.material.specular = mScene->mMaterials[i]->Get(AI_MATKEY_COLOR_SPECULAR, color) == AI_SUCCESS ?
			DirectX::XMFLOAT4(color.r, color.g, color.b, shininess) : DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, shininess);

		mMaterials.push_back(material);
	}
	DebugLog::Log("モデル読み込み完了");
	return true;
}

void PBRModel::LoadDefShader()
{
	mDefPS = std::make_shared<PixelShader>();
	mDefVS = std::make_shared<VertexShader>();

	mDefPS = SceneManager::Get()->GetObj<PixelShader>("PS_PBRModel");
	mDefVS = SceneManager::Get()->GetObj<VertexShader>("VS_PBRModel");
}

void PBRModel::LoadDefShader(const std::shared_ptr<VertexShader>& vsShader,
	const std::shared_ptr<PixelShader>& psShader)
{
	Primitive::LoadDefShader(vsShader, psShader);
}

void PBRModel::Update(float dt)
{
}

void PBRModel::Draw(int texSlot)
{
	SetDefShader();

	auto it = mMeshes.begin();
	while (it != mMeshes.end())
	{
		mVS->SetShader();
		mPS->SetShader();

		if(mMaterials[it->materialID].albedoTex)
			mPS->SetTexture(0, mMaterials[it->materialID].albedoTex.get());
		if (mMaterials[it->materialID].normalMap)
			mPS->SetTexture(1, mMaterials[it->materialID].normalMap.get());
		if (mMaterials[it->materialID].normalMap)
			mPS->SetTexture(2, mMaterials[it->materialID].metallicTex.get());
		it->mesh->Draw();
		++it;
	}
}

void PBRModel::WriteDefShader()
{
	if (!mDefPS || !mDefVS)
	{
		DebugLog::LogError("ShaderFile is not set");
		return;
	}

	CameraBase* firstCamera = GameApp::GetCurrentCamera();
	std::shared_ptr<DirLight> dirLight = SceneManager::Get()->GetObj<DirLight>("EnvironmentLight");

	XMFLOAT4X4 WVP[3] = {};
	//WORLD
	WVP[0] = mTransform.GetMatrixFX4();

	//VIEW
	WVP[1] = firstCamera->GetViewXMF();

	//PROJ
	WVP[2] = firstCamera->GetProjXMF();

	XMFLOAT4 eyePos = { firstCamera->GetPos().x,firstCamera->GetPos().y ,firstCamera->GetPos().z ,0.0f };

	struct constantBuffer
	{
		Vector4 eyePos;
		DirectX::XMFLOAT4 lightAmbient;
		DirectX::XMFLOAT4 lightDiffuse;
		DirectX::XMFLOAT4 lightDir;
	};

	constantBuffer cb = {
		eyePos,
		dirLight->GetAmbient(),
		dirLight->GetDiffuse(),
	Vector4{dirLight->GetPosition().x,dirLight->GetPosition().y,dirLight->GetPosition().z,0},
	};
	
	mDefVS->WriteShader(0, WVP);
	mDefPS->WriteShader(0, &cb);
}

void PBRModel::LoadAlbedoTex(std::shared_ptr<Texture> tex)
{
	if (mMaterials.size() == 1)
	{
		mMaterials[0].albedoTex = tex;
	}
}

void PBRModel::LoadNormalMapTex(std::shared_ptr<Texture> tex)
{
	if (mMaterials.size() == 1)
	{
		mMaterials[0].normalMap = tex;
	}
}

void PBRModel::LoadMetallicMapTex(std::shared_ptr<Texture> tex)
{
	if (mMaterials.size() == 1)
	{
		mMaterials[0].metallicTex = tex;
	}
}



void PBRModel::LoadTex(PBRConfig::PBRTexList list)
{
	LoadAlbedoTex(list[PBRConfig::PBRTex::ALBEDO]);
	LoadNormalMapTex(list[PBRConfig::PBRTex::NORMAL]);
	LoadMetallicMapTex(list[PBRConfig::PBRTex::METALLIC]);
}


