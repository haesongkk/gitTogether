#pragma once
class Mesh;
class Material;
class GameObject
{
	Matrix m_matrix = {};
	GameObject* m_pParentObject = nullptr;

	Vector3 m_position = { 0,0,0 };
	Vector3 m_scale = { 1,1,1 };
	Vector3 m_rotate = { 0,0,0 };

public:
	vector<Mesh*> m_pMeshes;
	vector<Material*> m_pMaterials;

public:
	void Init();
	void Update();
	void Render();
	void Final();

	Matrix GetMatrix() { return m_matrix; }

};

