#pragma once
class Node;
class Mesh;
class Bone
{
public:
	Mesh* m_pOwner;
	Node* m_pConnectNode;
	Matrix m_offsetMatrix;
	string m_nodeName;
	int m_index;

	void SetConnectNode(string name);
	void Update();
};