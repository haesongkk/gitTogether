#pragma once
class Model;
class Renderer;
class Material;
class Bone;
class Node;
class Vertex;
class Mesh
{
public:
	Mesh(Model* _pOwner);
	~Mesh();

	void CreateVertexBuffer(vector<Vertex>& _vertices);
	void CreateIndexBuffer(vector<WORD>& _indices);

	void SetMaterialIndex(UINT _index);

	void Render();
	
	static Renderer* pRenderer;
	Model* m_pOwner;
	Material* m_pConnectMaterial;
	Node* m_pParentNode = nullptr;

	ID3D11Buffer* pVB = nullptr;
	ID3D11Buffer* pIB = nullptr;

	UINT VertextBufferStride = 0; // 버텍스 하나의 크기.
	UINT VertextBufferOffset = 0;	// 버텍스 버퍼의 오프셋.
	UINT indexCount = 0;

	vector<Bone*> m_pBones;

};
