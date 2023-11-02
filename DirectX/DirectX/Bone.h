#pragma once
class Node;
class Mesh;
class Bone
{
public:
	string m_name;
	Mesh* m_pOwner;
	Node* m_pConnectNode;
	Matrix m_offsetMatrix;
	Matrix m_matrix;
	string m_nodeName;
	int m_index;

	void SetConnectNode(string name);
	void Update();
};

