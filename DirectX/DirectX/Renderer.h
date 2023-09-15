#pragma once
struct Vertex
{
	Vector3 position;		
	Vector4 color;			
	Vertex() : position(Vector3()),color(Vector4()) { }
	Vertex(Vector3 position, Vector4 color)
		: position(position), color(color) { }
};
class Object
{
public :
	vector<Vertex>& GetVertices() { return m_vertices; }
	vector<WORD>& GetIndicies() { return m_indices; }

	ID3D11Buffer*& GetVB() { return m_pVertexBuffer; }
	ID3D11Buffer*& GetIB() { return m_pIndexBuffer; }

	UINT& GetStride() { return m_VertextBufferStride; }
	UINT& GetOffset() { return m_VertextBufferOffset; }

	Matrix& GetMatrix() { return m_matrix; }
	Object*& GetParentObject() { return m_pParentObject; }

	Vector3& GetPos() { return m_position; }
	Vector3& GetScale() { return m_scale; }
	Vector3& GetRotate() { return m_rotate; }

private:
	ID3D11Buffer* m_pVertexBuffer = nullptr;
	ID3D11Buffer* m_pIndexBuffer = nullptr;

	UINT m_VertextBufferStride = 0;
	UINT m_VertextBufferOffset = 0;

	vector<Vertex> m_vertices = {};
	vector<WORD> m_indices = {};

	Matrix m_matrix;
	Object* m_pParentObject = nullptr;

	Vector3 m_position = { 0,0,0 };
	Vector3 m_scale = { 1,1,1 };
	Vector3 m_rotate = { 0,0,0 };

};
struct Camera
{
	Vector3 pos = { 0,1,5 };
	Vector3 focus = { 0,1,0 };
	Vector3 headDir = { 0,1,0 };
	Matrix viewMatrix;

	float fovY = 1.570796327f;
	float nearZ = 0.01f;
	float farZ = 100.0f;
	Matrix projMatrix;
};
class Renderer
{
public:
	virtual void Init(HINSTANCE hInstance);
	virtual void Run();
	virtual void Update();
	virtual void Render();
	virtual void Final();

	void InitWindow(HINSTANCE hInstance);
	void InitDX();
	void InitScene();
	void InitImGui();
	void InitObj();

protected:
	HWND m_hWnd;
	WNDCLASSEXW m_wcex;

	UINT m_width = 1024;
	UINT m_height = 768;

	ID3D11Device* m_pDevice = nullptr;
	ID3D11DeviceContext* m_pDeviceContext = nullptr;
	IDXGISwapChain* m_pSwapChain = nullptr;
	ID3D11RenderTargetView* m_pRenderTargetView = nullptr;
	ID3D11DepthStencilView* m_pDepthStencilView = nullptr;

	ID3D11VertexShader* m_pVertexShader = nullptr;
	ID3D11PixelShader* m_pPixelShader = nullptr;
	ID3D11InputLayout* m_pInputLayout = nullptr;

	ID3D11Buffer* m_pConstantBuffer = nullptr;

	vector<Object*> m_objects;
	Camera m_camera;

};

