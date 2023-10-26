#pragma once
class Mesh;
class Material;
class Animation;
class Renderer;
class GameObject;
class Node
{
public:
	static Renderer* pRenderer;

	Node(GameObject* _pOwner) : m_pOwner(_pOwner) { }
	GameObject* m_pOwner = nullptr;
	Node* m_pParent = nullptr;

	string m_name;

	Matrix m_worldMatrix = {};
	Matrix m_relativeMatrix = {};

	vector<Mesh*> m_pMeshes = {};
	vector<Node*> m_children = {};
	
	void Render();

};

