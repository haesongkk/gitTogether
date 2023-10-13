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
class Renderer;
class Mesh
{
public:
	void Init();
	void Update();
	void Render();
	void Final();

	void CreateVertexBuffer(vector<Vertex>& _vertices);
	void CreateIndexBuffer(vector<WORD>& _indices);
	void CreateResourceView(const wstring& _filePath, ID3D11ShaderResourceView** _ppRV);

	static Renderer* pRenderer;

	ID3D11Buffer* pVB = nullptr;
	ID3D11Buffer* pIB = nullptr;

	ID3D11ShaderResourceView* pTRV = nullptr;
	ID3D11ShaderResourceView* pNRV = nullptr;
	ID3D11ShaderResourceView* pSRV = nullptr;


	UINT VertextBufferStride = 0; // 버텍스 하나의 크기.
	UINT VertextBufferOffset = 0;	// 버텍스 버퍼의 오프셋.
	UINT indexCount = 0;

	Matrix matrix;
	Mesh* pParentObject = nullptr;

	Vector3 position = { 0,0,0 };
	Vector3 scale = { 1,1,1 };
	Vector3 rotate = { 0,0,0 };

};
