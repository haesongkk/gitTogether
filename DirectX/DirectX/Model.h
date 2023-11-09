#pragma once
class Mesh;
class Material;
class Animation;
class Node;
class Renderer;
class Model
{
	Matrix m_matrix = {};
	Model* m_pParentObject = nullptr;

public:
	Vector3 m_position = { 0,0,0 };
	Vector3 m_scale = { 1,1,1 };
	Vector3 m_rotate = { 0,0,0 };

	vector<Material*> m_pMaterials = {};
	vector<Mesh*> m_pMeshes = {};
	vector<Node*> m_pNodes = {};
	vector<Animation*> m_pAnimations = {};

	Node* m_pRootNode = nullptr;

public:
	void Render();

public:
	Matrix GetMatrix() { return m_matrix; }

private:
	void UpdateModelTM();
};

