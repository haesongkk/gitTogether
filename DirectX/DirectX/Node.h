#pragma once
class Mesh;
class Material;
class Animation;
class Renderer;
class GameObject;
class Node
{
public:
	string m_name;
	Node* m_pParent = nullptr;
	GameObject* m_pOwner = nullptr;
	Node(GameObject* _pOwner) : m_pOwner(_pOwner) { }

	Matrix m_worldMatrix = {};
	Matrix m_relativeMatrix = {};

	vector<Mesh*> m_pMeshes = {};
	vector<Node*> m_children = {};
	static Renderer* pRenderer;
	void Update();

};

