#pragma once
struct Vertex
{
	Vector3 position;
	Vector2 tex;
	Vector3 norm;
	Vector3 tangent;

	int boneIndices[4] = {};
	float boneWeights[4] = {};

	bool AddBoneData(int _index, float _weight);

	Vertex() : position(Vector3()), norm(Vector3()), tex(Vector2()) { }

	Vertex(Vector3 position, Vector3 normal, Vector2 tex)
		: position(position), norm(normal), tex(tex) { }
	Vertex(Vector3 position, Vector3 normal, Vector2 tex, Vector3 tangent)
		: position(position), norm(normal), tex(tex), tangent(tangent) { }
};

class Model;
class Renderer;
class Material;
class Bone;
class Node;
class Mesh
{
public:
	Mesh(Model* _pOwner);
	~Mesh();
	void Render();
	void Update();
	void CreateVertexBuffer(vector<Vertex>& _vertices);
	void CreateIndexBuffer(vector<WORD>& _indices);

	void SetMaterialIndex(UINT _index);
	
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
