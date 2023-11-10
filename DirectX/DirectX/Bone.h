#pragma once
class Node;
class Mesh;
class Bone
{
	int m_boneIndex;
	weak_ptr<Mesh> m_wpOwnerMesh;
	string m_connectedNodeName;
	Matrix m_offsetMatrix;

public:
	Bone(int boneIndex, shared_ptr<Mesh> wpOwnerMesh, string connectedNodeName, Matrix offsetMatrix);

public:
	~Bone() { }

public:
	void Run();

};