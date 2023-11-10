#pragma once

class Model;
class Mesh;
class Material;
class Animation;
class Node;
class Bone;
class Vertex;

struct aiMesh;
struct aiMaterial;
struct aiNodeAnim;
struct aiNode;
struct aiBone;

class FbxLoader
{
public:
	Model* LoadGameObject(const string& _filePath);
	shared_ptr<Mesh> CreateMesh(aiMesh* _mesh, shared_ptr<Model> _spModel);
	shared_ptr<Material> CreateMaterial(aiMaterial* matt, Model* obj);
	Animation* CreateAnimation(aiNodeAnim* nodeAnimation, Model* obj);
	Node* CreateNode(aiNode* aiNodeInfo, Node* parent, Model* obj);
};

