#pragma once
class Model;
class Mesh;
class Material;
class Animation;
class Node;
class Bone;
class Vertex;
class FbxLoader
{
public:
	Model* LoadGameObject(ID3D11Device* device, const string& _filePath);
	Mesh* CreateMesh(ID3D11Device* device, aiMesh* mesh, Model* obj);
	Material* CreateMaterial(ID3D11Device* device, aiMaterial* matt, Model* obj);
	Animation* CreateAnimation(aiNodeAnim* nodeAnimation, Model* obj);
	Node* CreateNode(aiNode* aiNodeInfo, Node* parent, Model* obj);
	Bone* CreateBone(aiBone* aiBone, Mesh* ownerMesh, vector<Vertex>& verticies, int boneIndex);
};

