#pragma once
struct Vertex
{
	Vector3 position;
	Vector2 tex;
	Vector3 norm;

	Vertex() : position(Vector3()), norm(Vector3()), tex(Vector2()) { }

	Vertex(Vector3 position, Vector3 normal, Vector2 tex)
		: position(position), norm(normal), tex(tex) { }
};
class Object
{
public:
	vector<Vertex>& GetVertices() { return m_vertices; }
	vector<WORD>& GetIndicies() { return m_indices; }

	ID3D11Buffer*& GetVB() { return m_pVertexBuffer; }
	ID3D11Buffer*& GetIB() { return m_pIndexBuffer; }
	ID3D11ShaderResourceView*& GetTRV() { return m_pTextureRV; }
	ID3D11SamplerState*& GetSL() { return m_pSamplerLinear; }

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

	ID3D11ShaderResourceView* m_pTextureRV = nullptr;
	ID3D11SamplerState* m_pSamplerLinear = nullptr;

	UINT m_VertextBufferStride = 0; // 버텍스 하나의 크기.
	UINT m_VertextBufferOffset = 0;	// 버텍스 버퍼의 오프셋.

	vector<Vertex> m_vertices = {};
	vector<WORD> m_indices = {};

	Matrix m_matrix;
	Object* m_pParentObject = nullptr;

	Vector3 m_position = { 0,0,0 };
	Vector3 m_scale = { 1,1,1 };
	Vector3 m_rotate = { 0,0,0 };

};

struct TransformBuffer
{
	Matrix mWorld;
	Matrix mView;
	Matrix mProjection;
};

struct LightBuffer
{
	Vector3 Direction = { 0.0f, 0.0f, 1.0f };
	float pad0;
	Vector4 Ambient = { 0.1f,0.1f,0.1f,0.1f };
	Vector4 Diffuse = { 1.0f,1.0f,1.0f,1.0f };
	Vector4 Specular = { 1.0f,1.0f,1.0f,1.0f };
	Vector3 EyePosition;
	bool UseBlinnPhong = true;
};

struct MarterialBuffer
{
	Vector4 Ambient = { 1.0f,1.0f,1.0f,1.0f };
	Vector4 Diffuse = { 1.0f,1.0f,1.0f,1.0f };
	Vector4 Specular = { 1.0f,1.0f,1.0f,1.0f };
	float  SpecularPower = 2000;
	Vector3 dummy;
};

struct DirectionalLight
{
	Vector4 color = { 0.5f, 0.5f, 0.5f, 1.0f };
	Vector4 dir = { 0.0f, -1.0f, 0.f, 1.0f };
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

	ID3D11Buffer* m_pTransformBuffer = nullptr;
	ID3D11Buffer* m_pLightBuffer = nullptr;
	ID3D11Buffer* m_pMaterialBuffer = nullptr;

	TransformBuffer m_TB;
	LightBuffer m_LB;
	MarterialBuffer m_MB;

	vector<Object*> m_objects;
	struct Camera
	{
		Vector3 pos = { 0,3,-5 };
		Vector3 dir = { 0,-0.5,1 };
		Vector3 headDir = { 0,1,0 };
		Matrix viewMatrix;

		float fovY = 1.570796327f;
		float nearZ = 0.01f;
		float farZ = 100.0f;
		Matrix projMatrix;
	} m_camera;
	DirectionalLight m_dirLight;

};

