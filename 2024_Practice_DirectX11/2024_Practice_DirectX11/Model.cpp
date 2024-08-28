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
	DebugLog::Log("���f���ǂݍ��݊J�n");

	int flag = 0;
	if (simpleMode)
	{
		flag |= aiProcess_Triangulate;					// ��O�p�|���S�����O�p�Ɋ���
		flag |= aiProcess_JoinIdenticalVertices;		// ����ʒu���_����ɓ�������
		flag |= aiProcess_FlipUVs;						//�@UV�l��Y������ɔ��]������
		flag |= aiProcess_PreTransformVertices;			// �m�[�h����ɓ��� !!�A�j���[�V������񂪏����邱�Ƃɒ���!!
		if (flip) flag |= aiProcess_MakeLeftHanded;		// ����n���W�ɕϊ�
	}
	else
	{
		flag |= aiProcessPreset_TargetRealtime_MaxQuality;	// ���A���^�C�� �����_�����O�p�Ƀf�[�^���œK������f�t�H���g�̌㏈���\���B
		flag |= aiProcess_PopulateArmatureData;				// �W���I�ȃ{�[��,�A�[�}�`���A�̐ݒ�
		if (flip) flag |= aiProcess_ConvertToLeftHanded;	// ����n�ύX�I�v�V�������܂Ƃ܂�������
	}

	// assimp�œǂݍ���
	mScene=importer.get()->ReadFile(file, flag);
	if (!mScene) {
		Error(importer->GetErrorString());
		DebugLog::Log("{} Assimp���f�����[�h���s", file);
		return false;
	}

	// ���b�V���̍쐬
	aiVector3D zero(0.0f, 0.0f, 0.0f);
	for (unsigned int i = 0; i < mScene->mNumMeshes; ++i)
	{
		MeshBuffer mesh = {};

		// ���_�̍쐬
		std::vector<Vertex::VtxPosNormalTex> vtx;
		vtx.resize(mScene->mMeshes[i]->mNumVertices);
		for (unsigned int j = 0; j < vtx.size(); ++j)
		{
			// �l�̋z�o��
			aiVector3D pos = mScene->mMeshes[i]->mVertices[j];
			aiVector3D uv = mScene->mMeshes[i]->HasTextureCoords(0) ?
				mScene->mMeshes[i]->mTextureCoords[0][j] : zero;
			aiVector3D normal = mScene->mMeshes[i]->HasNormals() ?
				mScene->mMeshes[i]->mNormals[j] : zero;
			// �l��ݒ�
			vtx[j] = {
				DirectX::XMFLOAT3(pos.x, pos.y, pos.z),
				DirectX::XMFLOAT3(normal.x, normal.y, normal.z),
				DirectX::XMFLOAT2(uv.x, uv.y),
				/*{-1,-1,-1,-1},
				{0.0f,0.0f,0.0f,0.0f}*/
			};
		}

		// �C���f�b�N�X�̍쐬
		std::vector<unsigned int> idx;
		idx.resize(mScene->mMeshes[i]->mNumFaces * 3); // face�̓|���S���̐���\��(�P�|���S����3�C���f�b�N�X
		for (unsigned int j = 0; j < mScene->mMeshes[i]->mNumFaces; ++j)
		{
			aiFace face = mScene->mMeshes[i]->mFaces[j];
			int faceIdx = j * 3;
			idx[faceIdx + 0] = face.mIndices[0];
			idx[faceIdx + 1] = face.mIndices[1];
			idx[faceIdx + 2] = face.mIndices[2];
		}

		// �}�e���A���̊��蓖��
		mesh.materialID = mScene->mMeshes[i]->mMaterialIndex;

		// ���b�V�������ɒ��_�o�b�t�@�쐬
		Mesh::MeshData desc = {};
		desc.pVertex = vtx.data();
		desc.vertexSize = sizeof(Vertex::VtxPosNormalTex);
		desc.vertexCount = static_cast<UINT>(vtx.size());
		desc.pIndex = idx.data();
		desc.indexSize = sizeof(unsigned int);
		desc.indexCount = static_cast<UINT>(idx.size());
		desc.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		mesh.mesh = std::make_shared<Mesh>(desc);

		// ���b�V���ǉ�
		mMeshes.push_back(mesh);
	}

	//--- �}�e���A���̍쐬
	// �t�@�C���̒T��
	std::string dir = file;
	dir = dir.substr(0, dir.find_last_of('/') + 1);
	// �}�e���A��
	aiColor3D color(0.0f, 0.0f, 0.0f);
	DirectX::XMFLOAT4 diffuse(1.0f, 1.0f, 1.0f, 1.0f);
	DirectX::XMFLOAT4 ambient(0.3f, 0.3f, 0.3f, 1.0f);
	for (unsigned int i = 0; i < mScene->mNumMaterials; ++i)
	{
		MaterialData material = {};

		// �e��p�����[�^�[
		float shininess;
		material.material.diffuse = mScene->mMaterials[i]->Get(AI_MATKEY_COLOR_DIFFUSE, color) == AI_SUCCESS ?
			DirectX::XMFLOAT4(color.r, color.g, color.b, 1.0f) : diffuse;
		material.material.ambient = mScene->mMaterials[i]->Get(AI_MATKEY_COLOR_AMBIENT, color) == AI_SUCCESS ?
			DirectX::XMFLOAT4(color.r, color.g, color.b, 1.0f) : ambient;
		shininess = mScene->mMaterials[i]->Get(AI_MATKEY_SHININESS, shininess) == AI_SUCCESS ? shininess : 0.0f;
		material.material.specular = mScene->mMaterials[i]->Get(AI_MATKEY_COLOR_SPECULAR, color) == AI_SUCCESS ?
			DirectX::XMFLOAT4(color.r, color.g, color.b, shininess) : DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, shininess);

		// �e�N�X�`��
		aiString path;
		if (mScene->mMaterials[i]->Get(AI_MATKEY_TEXTURE_DIFFUSE(0), path) == AI_SUCCESS) {
			HRESULT hr;
			// �t�@�C���`���`�F�b�N
			if (strstr(path.C_Str(), ".psd"))
			{
				DebugLog::LogError("���f���̃e�N�X�`����psd�t�@�C�����w�肳��Ă��܂��Bpsd�ǂݍ��݂ɂ͔�Ή��B");
				Error("���f���̃e�N�X�`����psd�t�@�C�����w�肳��Ă��܂��Bpsd�ǂݍ��݂ɂ͔�Ή��B");
			}
			// ���̂܂ܒT��
			material.tex = std::make_shared<Texture>();
			hr = material.tex->Create(path.C_Str());
			// ���f���Ɠ����K�w��T��
			if (FAILED(hr)) {
				std::string file = dir;
				file += path.C_Str();
				hr = material.tex->Create(file.c_str());
			}
			// �t�@�C�����݂̂ŒT��
			if (FAILED(hr)) {
				std::string file = path.C_Str();
				if (size_t idx = file.find_last_of('\\'); idx != std::string::npos)
				{
					file = file.substr(idx + 1);
					file = dir + file;
					hr = material.tex->Create(file.c_str());
				}

				// �t�@�C���ڂ̃p�X��"\\�ł͂Ȃ�"/"�̏ꍇ�̑Ή�
				if (FAILED(hr)) {
					if (size_t idx = file.find_last_of('/'); idx != std::string::npos)
					{
						file = file.substr(idx + 1);
						file = dir + file;
						hr = material.tex->Create(file.c_str());
					}
				}
			}
			// ���s
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

		// �}�e���A���ǉ�
		mMaterials.push_back(material);
	}

	DebugLog::Log("���f���ǂݍ��݊���");
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
