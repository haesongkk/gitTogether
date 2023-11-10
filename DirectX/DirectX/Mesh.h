#pragma once
class Model;
class Bone;
class Node;
class Vertex;
class Mesh
{
	weak_ptr<Model> m_wpOwnerModel;
	int m_connectedMaterialIndex;

	wpVector<Bone> m_wpBones;

	Node* m_pParentNode = nullptr;

	ID3D11Buffer* m_pVertexBuffer;
	ID3D11Buffer* m_pIndexBuffer;
	ID3D11Buffer* m_pBonesBuffer;

	int VertextBufferStride = 0;
	int VertextBufferOffset = 0;
	int indexCount = 0;

	struct CB_BonePallete
	{
		Matrix bonePallete[128] = {};
	} m_bonePallete;

public:
	Mesh(shared_ptr<Model> _spOwnerModel);
	Mesh();

public:
	~Mesh();

	void CreateVertexBuffer(vector<Vertex>& _vertices);
	void CreateIndexBuffer(vector<WORD>& _indices);


	void Run();

public:
	shared_ptr<Model> GetOwnerModel() { return m_wpOwnerModel.lock(); }

public:
	void SetMeshInfo(shared_ptr<Model> _spOwnerModel, int _connectedMaterialIndex, vector<Bone> _wpBones, vector<Vertex> _vertices, vector<WORD> _indices);

	void BonePallete(int index, Matrix matrix);
};
