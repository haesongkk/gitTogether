#pragma once
struct Vertex
{
	Vector3 position;		
	Vector4 color;			
	Vertex() 
		: position(Vector3()),color(Vector4()) { }
	Vertex(Vector3 position, Vector4 color)
		: position(position), color(color) { }
};
class Renderer
{
public:
	virtual void Init(HINSTANCE hInstance);
	virtual void Run();
	virtual void Update();
	virtual void Render();
	virtual void Final();

	void InitWindow();
	void InitDX();
	void InitScene();

	void SetPS(LPCWSTR _ps);
	void SetVS(LPCWSTR _vs);
	void SetnVertices(UINT _nV);
	void SetVertices(UINT _i, Vertex _v);
	void SetnIndices(UINT _nI);
	void SetIndicies(UINT _i, WORD _w);

protected:
	HINSTANCE m_inst;
	HWND m_hWnd;
	WNDCLASSEXW m_wcex;

	LPCWSTR m_class = L"DefaultWindowCalss";
	LPCWSTR m_title = L"GameApp";

	UINT m_width = 1024;
	UINT m_height = 768;

	ID3D11Device* m_pDevice = nullptr;
	ID3D11DeviceContext* m_pDeviceContext = nullptr;
	IDXGISwapChain* m_pSwapChain = nullptr;
	ID3D11RenderTargetView* m_pRenderTargetView = nullptr;

	ID3D11VertexShader* m_pVertexShader = nullptr;
	ID3D11PixelShader* m_pPixelShader = nullptr;
	ID3D11InputLayout* m_pInputLayout = nullptr;
	ID3D11Buffer* m_pVertexBuffer = nullptr;
	ID3D11Buffer* m_pIndexBuffer = nullptr;

	UINT m_VertextBufferStride = 0;
	UINT m_VertextBufferOffset = 0;

	LPCWSTR m_psFileName = nullptr;
	LPCWSTR m_vsFileName = nullptr;

	UINT m_nVertices = 0;
	Vertex* m_vertices = nullptr;

	UINT m_nIndices = 0;
	WORD* m_indices = nullptr;
};

