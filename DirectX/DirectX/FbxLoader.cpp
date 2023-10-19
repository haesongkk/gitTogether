#include "framework.h"
#include "FbxLoader.h"
#include "GameObject.h"
#include "Mesh.h"
#include "Material.h"
#include "Helper.h"

GameObject* FbxLoader::LoadGameObject(ID3D11Device* device, const string& _filePath)
{
	GameObject* pGameObject = new GameObject;

	Assimp::Importer importer;
	unsigned int importFlags = aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_GenUVCoords | aiProcess_CalcTangentSpace |
		aiProcess_ConvertToLeftHanded;

	vector<Mesh*> pMeshes;
	vector<Material*> pMaterials;

	const aiScene* scene = importer.ReadFile(_filePath.c_str(), importFlags);
	assert(scene);

	for (int i = 0; i < scene->mNumMeshes; ++i)
		pMeshes.push_back(CreateMesh(device, scene->mMeshes[i], pGameObject));
	
	for (int i = 0; i < scene->mNumMaterials; ++i)
		pMaterials.push_back(CreateMaterial(device, scene->mMaterials[i], pGameObject));

	importer.FreeScene();

	pGameObject->m_pMeshes = pMeshes;
	pGameObject->m_pMaterials = pMaterials;

	return pGameObject;
}

Mesh* FbxLoader::CreateMesh(ID3D11Device* _device, aiMesh* _mesh, GameObject* _obj)
{
	Mesh* mesh = new Mesh(_obj);

	mesh->SetMaterialIndex(_mesh->mMaterialIndex);

	vector<Vertex> verticies;
	for (UINT i = 0; i < _mesh->mNumVertices; ++i)
	{
		Vertex vertex;
		memcpy_s(&vertex.position, sizeof(vertex.position), &_mesh->mVertices[i], sizeof(_mesh->mVertices[i]));
		memcpy_s(&vertex.norm, sizeof(vertex.norm), &_mesh->mNormals[i], sizeof(_mesh->mNormals[i]));
		memcpy_s(&vertex.tex, sizeof(Vector2), &_mesh->mTextureCoords[0][i], sizeof(Vector2));
		memcpy_s(&vertex.tangent, sizeof(vertex.tangent), &_mesh->mTangents[i], sizeof(_mesh->mTangents[i]));
		verticies.push_back(vertex);
	}
	mesh->CreateVertexBuffer(verticies);

	vector<WORD> indices;
	for (UINT i = 0; i < _mesh->mNumFaces; ++i)
	{
		indices.push_back(_mesh->mFaces[i].mIndices[0]);
		indices.push_back(_mesh->mFaces[i].mIndices[1]);
		indices.push_back(_mesh->mFaces[i].mIndices[2]);
	}
	mesh->CreateIndexBuffer(indices);

	return mesh;
}

Material* FbxLoader::CreateMaterial(ID3D11Device* device, aiMaterial* _pMaterial, GameObject* _obj)
{
	Material* material = new Material(_obj);

	aiString texturePath;
	wstring basePath = L"./Resource/";

	filesystem::path path;
	wstring finalPath;
	string name = _pMaterial->GetName().C_Str();


	if (AI_SUCCESS == _pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &texturePath))
	{
		path = Helper::ToWstring(string(texturePath.C_Str()));
		finalPath = basePath + path.filename().wstring();
		material->CreateTextureFromFile(finalPath.c_str(), Material::TextureIndex::Diffuse);
	}

	if (AI_SUCCESS == _pMaterial->GetTexture(aiTextureType_NORMALS, 0, &texturePath))
	{
		path = Helper::ToWstring(string(texturePath.C_Str()));
		finalPath = basePath + path.filename().wstring();
		material->CreateTextureFromFile(finalPath.c_str(), Material::TextureIndex::Normal);
	}

	if (AI_SUCCESS == _pMaterial->GetTexture(aiTextureType_SPECULAR, 0, &texturePath))
	{
		path = Helper::ToWstring(string(texturePath.C_Str()));
		finalPath = basePath + path.filename().wstring();
		material->CreateTextureFromFile(finalPath.c_str(), Material::TextureIndex::Specular);
	}

	if (AI_SUCCESS == _pMaterial->GetTexture(aiTextureType_EMISSIVE, 0, &texturePath))
	{
		path = Helper::ToWstring(string(texturePath.C_Str()));
		finalPath = basePath + path.filename().wstring();
		material->CreateTextureFromFile(finalPath.c_str(), Material::TextureIndex::Emissive);
	}

	if (AI_SUCCESS == _pMaterial->GetTexture(aiTextureType_OPACITY, 0, &texturePath))
	{
		path = Helper::ToWstring(string(texturePath.C_Str()));
		finalPath = basePath + path.filename().wstring();
		material->CreateTextureFromFile(finalPath.c_str(), Material::TextureIndex::Opacity);
	}
	return material;
}
