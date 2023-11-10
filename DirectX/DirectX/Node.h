#pragma once
class Mesh;
class Material;
class Animation;
class Renderer;
class Model;
class Node
{
	string m_name;

	Matrix m_worldMatrix = {};
	Matrix m_relativeMatrix = {};
	
	weak_ptr<Model> m_wpOwnerModel;

	string m_parentNodeName;
	vector<string> m_childrenNodeNames;
	vector<int> m_connectedMeshIndicies;

	ID3D11Buffer* m_pWorldTMBuffer;

	struct CB_WorldTM
	{
		Matrix worldTM;
	} m_worldTM;

public:
	Node(string name, shared_ptr<Model> spOwnerModel, Matrix localTM, vector<int> connectedMeshIndices, vector<string> childrenNodeNames);

public:
	~Node();

public:
	void Run();

public:
	string GetName() { return m_name; }
	Matrix GetMatrix() { return m_worldMatrix; }

};