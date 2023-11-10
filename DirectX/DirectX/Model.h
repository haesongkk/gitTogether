#pragma once
class Mesh;
class Material;
class Animation;
class Node;
class Renderer;
class Transform;
#include "Component.h"
class Model : public Component
{
	weak_ptr<Transform> m_wpOwnerTransform;

	wpVector<Material> m_wpMaterials;
	wpVector<Mesh> m_wpMeshes;
	wpVector<Node> m_wpNodes;

	map<string, wpVector<Animation>> m_wpAnimations;
	string m_curAnim;

	weak_ptr<Node> m_wpRootNode;

public:
	Model();

public:
	virtual ~Model();

public:
	virtual void Run();

public:
	shared_ptr<Material> GetMaterial(int index) { return m_wpMaterials[index].lock(); }
	shared_ptr<Mesh> GetMesh(int index) { return m_wpMeshes[index].lock(); }
	shared_ptr<Node> GetNode(string name);
	

	shared_ptr<Transform> GetTransform() { return m_wpOwnerTransform.lock(); }

	static weak_ptr<Renderer> m_wpRenderer;
	friend class FbxLoader;

};

