#pragma once
class GameObject;
class Mesh;
class Material;
class FbxLoader
{
public:
	GameObject* LoadGameObject(ID3D11Device* device, const string& _filePath);
	Mesh* CreateMesh(ID3D11Device* device, aiMesh* mesh, GameObject* obj);
	Material* CreateMaterial(ID3D11Device* device, aiMaterial* matt, GameObject* obj);
};

