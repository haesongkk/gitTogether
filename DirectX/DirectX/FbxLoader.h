#pragma once
class GameObject;
class Mesh;
class Material;
class Animation;
class Node;
class FbxLoader
{
public:
	GameObject* LoadGameObject(ID3D11Device* device, const string& _filePath);
	Mesh* CreateMesh(ID3D11Device* device, aiMesh* mesh, GameObject* obj);
	Material* CreateMaterial(ID3D11Device* device, aiMaterial* matt, GameObject* obj);
	Animation* CreateAnimation(aiNodeAnim* nodeAnimation, GameObject* obj);
	Node* CreateNode(aiNode* aiNodeInfo, Node* parent, GameObject* obj);
};

