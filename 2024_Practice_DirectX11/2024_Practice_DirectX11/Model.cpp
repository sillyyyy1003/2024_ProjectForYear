#include "Model.h"
#include <memory>
#include "DebugLog.h"
#include "Shader.h"
#include <assimp/postprocess.h>

#ifdef _DEBUG
#pragma comment(lib, "assimp-vc143-mtd.lib")
#else
#pragma comment(lib, "assimp-vc143-mt.lib")
#endif

Model::Model()
{
	importer = std::make_unique<Assimp::Importer>();
}

Model::~Model()
{
}

void Model::SetVertexShader(VertexShader* vs)
{
	mVS = vs;
}

void Model::SetPixelShader(PixelShader* ps)
{
	mPS = ps;
}

bool Model::Load(const char* file, bool flip, bool simpleMode)
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
	mScene=importer.get()->ReadFile(file, flag);
	if (!mScene) {
		Error(importer->GetErrorString());
		DebugLog::Log("{} Assimpモデルロード失敗", file);
		return false;
	}

	// メッシュの作成
	aiVector3D zero(0.0f, 0.0f, 0.0f);
	for (unsigned int i = 0; i < mScene->mNumMeshes; ++i)
	{
		MeshBuffer mesh = {};

		// 頂点の作成
		std::vector<Vertex::VtxPosNormalTex> vtx;
		vtx.resize(mScene->mMeshes[i]->mNumVertices);
		for (unsigned int j = 0; j < vtx.size(); ++j)
		{
			// 値の吸出し
			aiVector3D pos = mScene->mMeshes[i]->mVertices[j];
			aiVector3D uv = mScene->mMeshes[i]->HasTextureCoords(0) ?
				mScene->mMeshes[i]->mTextureCoords[0][j] : zero;
			aiVector3D normal = mScene->mMeshes[i]->HasNormals() ?
				mScene->mMeshes[i]->mNormals[j] : zero;
			// 値を設定
			vtx[j] = {
				DirectX::XMFLOAT3(pos.x, pos.y, pos.z),
				DirectX::XMFLOAT3(normal.x, normal.y, normal.z),
				DirectX::XMFLOAT2(uv.x, uv.y),
				/*{-1,-1,-1,-1},
				{0.0f,0.0f,0.0f,0.0f}*/
			};
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
		desc.vertexSize = sizeof(Vertex::VtxPosNormalTex);
		desc.vertexCount = static_cast<UINT>(vtx.size());
		desc.pIndex = idx.data();
		desc.indexSize = sizeof(unsigned int);
		desc.indexCount = static_cast<UINT>(idx.size());
		desc.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		mesh.mesh = std::make_shared<Mesh>(desc);

		// メッシュ追加
		mMeshes.push_back(mesh);
	}

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
		MaterialData material = {};

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
		if (mScene->mMaterials[i]->Get(AI_MATKEY_TEXTURE_DIFFUSE(0), path) == AI_SUCCESS) {
			HRESULT hr;
			// ファイル形式チェック
			if (strstr(path.C_Str(), ".psd"))
			{
				DebugLog::LogError("モデルのテクスチャにpsdファイルが指定されています。psd読み込みには非対応。");
				Error("モデルのテクスチャにpsdファイルが指定されています。psd読み込みには非対応。");
			}
			// そのまま探索
			material.tex = std::make_shared<Texture>();
			hr = material.tex->Create(path.C_Str());
			// モデルと同じ階層を探索
			if (FAILED(hr)) {
				std::string file = dir;
				file += path.C_Str();
				hr = material.tex->Create(file.c_str());
			}
			// ファイル名のみで探索
			if (FAILED(hr)) {
				std::string file = path.C_Str();
				if (size_t idx = file.find_last_of('\\'); idx != std::string::npos)
				{
					file = file.substr(idx + 1);
					file = dir + file;
					hr = material.tex->Create(file.c_str());
				}

				// ファイル目のパスが"\\ではなく"/"の場合の対応
				if (FAILED(hr)) {
					if (size_t idx = file.find_last_of('/'); idx != std::string::npos)
					{
						file = file.substr(idx + 1);
						file = dir + file;
						hr = material.tex->Create(file.c_str());
					}
				}
			}
			// 失敗
			if (FAILED(hr)) {
				Error(path.C_Str());
				material.tex = nullptr;
				material.material.isTexEnable = false;
			}
		}
		else {
			material.tex = nullptr;
			material.material.isTexEnable = false;
		}

		// マテリアル追加
		mMaterials.push_back(material);
	}

	DebugLog::Log("モデル読み込み完了");
	return true;
}

void Model::Draw(int texSlot)
{
	auto it = mMeshes.begin();

	//Set Bone Animation
	//GetContext()->VSSetConstantBuffers(5, 1, &m_BoneCombMtxCBuffer);

	while (it != mMeshes.end())
	{
		mPS->WriteShader(0, &mMaterials[it->materialID]);
		mVS->SetShader();
		mPS->SetShader();

		if (texSlot >= 0)
			mPS->SetTexture(texSlot, mMaterials[it->materialID].tex.get());

		it->mesh->Draw();
		++it;
	}
}
