#pragma once
struct Vertex
{
	Vector3 position;
	Vector2 tex;
	Vector3 norm;
	Vector3 tangent;

	Vertex() : position(Vector3()), norm(Vector3()), tex(Vector2()) { }

	Vertex(Vector3 position, Vector3 normal, Vector2 tex)
		: position(position), norm(normal), tex(tex) { }
	Vertex(Vector3 position, Vector3 normal, Vector2 tex, Vector3 tangent)
		: position(position), norm(normal), tex(tex), tangent(tangent) { }
};
class GameObject;
class Renderer;
class Material
{
public:
	static Renderer* pRenderer;
	GameObject* m_pOwner;

	enum TextureIndex
	{
		Diffuse, Normal, Specular, Emissive, Opacity, End,
	};

	ID3D11ShaderResourceView* m_pTextures[TextureIndex::End];

public:
	Material(GameObject* _pOwner);
	void Init();
	void Render();
	void Final();

	void CreateTextureFromFile(const wstring& _filePath, TextureIndex _txId);
};
class Mesh
{
public:
	Mesh(GameObject* _pOwner);
	void Init();
	void Render();
	void Final();

	void CreateVertexBuffer(vector<Vertex>& _vertices);
	void CreateIndexBuffer(vector<WORD>& _indices);

	void SetMaterialIndex(UINT _index) { m_materialIndex = _index; }
	

	static Renderer* pRenderer;
	GameObject* m_pOwner;

	ID3D11Buffer* pVB = nullptr;
	ID3D11Buffer* pIB = nullptr;

	UINT VertextBufferStride = 0; // ���ؽ� �ϳ��� ũ��.
	UINT VertextBufferOffset = 0;	// ���ؽ� ������ ������.
	UINT indexCount = 0;
	UINT m_materialIndex = 0;

};