#include "framework.h"
#include "FbxLoader.h"

#include "Model.h"
#include "Mesh.h"
#include "Material.h"
#include "Animation.h"
#include "Node.h"
#include "Helper.h"
#include "Bone.h"

Model* FbxLoader::LoadGameObject(ID3D11Device* device, const string& _filePath)
{
	Model* pGameObject = new Model;

	Assimp::Importer importer;
	unsigned int importFlags = aiProcess_Triangulate
		| aiProcess_GenNormals
		| aiProcess_GenUVCoords
		| aiProcess_CalcTangentSpace
		| aiProcess_LimitBoneWeights
		| aiProcess_ConvertToLeftHanded;

	vector<Mesh*> pMeshes;
	vector<Material*> pMaterials;
	vector<Animation*> pAnimations;

	const aiScene* scene = importer.ReadFile(_filePath.c_str(), importFlags);
	assert(scene);

	for (int i = 0; i < scene->mNumMaterials; ++i)
		pMaterials.push_back(CreateMaterial(device, scene->mMaterials[i], pGameObject));
	pGameObject->m_pMaterials = pMaterials;

	for (int i = 0; i < scene->mNumMeshes; ++i)
		pMeshes.push_back(CreateMesh(device, scene->mMeshes[i], pGameObject));
	pGameObject->m_pMeshes = pMeshes;
	
	pGameObject->m_pRootNode = CreateNode(scene->mRootNode, nullptr, pGameObject);


	for (int i = 0; i < scene->mNumAnimations; i++)
		for (int j = 0; j < scene->mAnimations[i]->mNumChannels; j++)
			pAnimations.push_back(CreateAnimation(scene->mAnimations[i]->mChannels[j], pGameObject));
	pGameObject->m_pAnimations = pAnimations;



	importer.FreeScene();

	return pGameObject;
}

Mesh* FbxLoader::CreateMesh(ID3D11Device* _device, aiMesh* _mesh, Model* _obj)
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

	vector<Bone*> bones = {};
	for (int i = 0; i < _mesh->mNumBones; i++)
		bones.push_back(CreateBone(_mesh->mBones[i], mesh, verticies,i));
	mesh->m_pBones = bones;

	mesh->CreateVertexBuffer(verticies);


	vector<WORD> indices;
	for (UINT i = 0; i < _mesh->mNumFaces; ++i)
	{
		indices.push_back(_mesh->mFaces[i].mIndices[0]);
		indices.push_back(_mesh->mFaces[i].mIndices[1]);
		indices.push_back(_mesh->mFaces[i].mIndices[2]);

		/*indices.push_back(_mesh->mFaces[i].mIndices[0]);
		indices.push_back(_mesh->mFaces[i].mIndices[2]);
		indices.push_back(_mesh->mFaces[i].mIndices[1]);*/
	}
	mesh->CreateIndexBuffer(indices);


	return mesh;
}

Material* FbxLoader::CreateMaterial(ID3D11Device* device, aiMaterial* _pMaterial, Model* _obj)
{
	Material* material = new Material(_obj);

	aiString texturePath;
	wstring basePath = L"./Resource/";

	filesystem::path path;
	wstring finalPath;
	string name = _pMaterial->GetName().C_Str();

	aiColor3D color(0.f, 0.f, 0.f);
	if (AI_SUCCESS == _pMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, color))
	{
		material->m_baseColor = { color.r,color.g,color.b,1.f };
	}

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

Animation* FbxLoader::CreateAnimation(aiNodeAnim* nodeAnimation, Model* obj)
{
	assert(nodeAnimation != nullptr);
	assert(nodeAnimation->mNumPositionKeys == nodeAnimation->mNumRotationKeys);
	assert(nodeAnimation->mNumRotationKeys == nodeAnimation->mNumScalingKeys);

	Animation* animation = new Animation(obj);


	for (size_t i = 0; i < nodeAnimation->mNumPositionKeys; i++)
	{
		Animation::Key key;

		aiVectorKey& pos = nodeAnimation->mPositionKeys[i];
		aiQuatKey& rot = nodeAnimation->mRotationKeys[i];
		aiVectorKey& scl = nodeAnimation->mScalingKeys[i];

		assert(pos.mTime == rot.mTime);
		assert(rot.mTime == scl.mTime);

		key.time = pos.mTime;
		key.position = Vector3(pos.mValue.x, pos.mValue.y, pos.mValue.z);
		key.rotation = Quaternion(rot.mValue.x, rot.mValue.y, rot.mValue.z, rot.mValue.w);
		key.scale = Vector3(scl.mValue.x, scl.mValue.y, scl.mValue.z);

		animation->m_keys.push_back(key);
	}

	for (auto node : obj->m_pNodes)
		if (node->m_name == nodeAnimation->mNodeName.C_Str())
			animation->m_pConnectNode = node;

	return animation;
}

Node* FbxLoader::CreateNode(aiNode* aiNodeInfo, Node* parent, Model* obj)
{
	Node* node = new Node(obj);
	obj->m_pNodes.push_back(node);

	node->m_name = aiNodeInfo->mName.C_Str();
	node->m_pParent = parent;
	node->m_relativeMatrix = XMMatrixTranspose(XMMATRIX(aiNodeInfo->mTransformation[0]));

	for (int i = 0; i < aiNodeInfo->mNumMeshes; i++)
	{
		obj->m_pMeshes[aiNodeInfo->mMeshes[i]]->m_pParentNode = node;
		node->m_pMeshes.push_back(obj->m_pMeshes[aiNodeInfo->mMeshes[i]]);
	}

	for (int i = 0; i < aiNodeInfo->mNumChildren; i++)
		node->m_children.push_back(CreateNode(aiNodeInfo->mChildren[i], node, obj));

	return node;
}

Bone* FbxLoader::CreateBone(aiBone* aiBone, Mesh* ownerMesh, vector<Vertex>& vertices, int boneIndex)
{
	Bone* bone = new Bone;
	bone->m_name = aiBone->mName.C_Str();
	bone->m_offsetMatrix = Matrix(&aiBone->mOffsetMatrix.a1).Transpose();
	bone->m_pOwner = ownerMesh;

	if(aiBone->mNode)
		bone->m_nodeName = aiBone->mNode->mName.C_Str();

	for (int i = 0; i < aiBone->mNumWeights; i++)
		assert(vertices[aiBone->mWeights[i].mVertexId].AddBoneData(boneIndex, aiBone->mWeights[i].mWeight));

	return bone;
}
