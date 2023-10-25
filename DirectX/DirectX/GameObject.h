#pragma once
class Mesh;
class Material;
class Animation;
class Node;
class GameObject
{
	Matrix m_matrix = {};

public:
	Vector3 m_position = { 0,0,0 };
	Vector3 m_scale = { 1,1,1 };
	Vector3 m_rotate = { 0,0,0 };

	vector<Mesh*> m_pMeshes;
	vector<Material*> m_pMaterials;
	vector<Animation*> m_pAnimations;
	vector<Node*> m_pNodes;
	Node* m_pRootNode;

public:
	void Init();
	void Update();
	void Render();
	void Final();

	Matrix GetMatrix() { return m_matrix; }

};

