#pragma once
class GameObject;
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

	void UpdateScene();

	void RenderScene();
	void RenderImGui();

	void FinalImGui();
	void FianlScene();
	void FinalDX();


private:
	HWND m_hWnd;
	WNDCLASSEXW m_wcex;

	UINT m_width = 1024;
	UINT m_height = 768;

	ID3D11Device* m_pDevice = nullptr;
	ID3D11DeviceContext* m_pDeviceContext = nullptr;
	IDXGISwapChain* m_pSwapChain = nullptr;
	ID3D11RenderTargetView* m_pRenderTargetView = nullptr;
	ID3D11DepthStencilView* m_pDepthStencilView = nullptr;
	ID3D11BlendState* m_pAlphaBlendState = nullptr;

	ID3D11VertexShader* m_pVertexShader = nullptr;
	ID3D11PixelShader* m_pPixelShader = nullptr;
	ID3D11InputLayout* m_pInputLayout = nullptr;
	ID3D11SamplerState* m_pSamplerLinear = nullptr;

	ID3D11Buffer* m_pTransformBuffer = nullptr;
	ID3D11Buffer* m_pLightBuffer = nullptr;
	ID3D11Buffer* m_pMaterialBuffer = nullptr;
	ID3D11Buffer* m_pUsingBuffer = nullptr;

	vector<GameObject*> m_pGameObjects;

	struct TransformBuffer
	{
		Matrix mWorld;
		Matrix mView;
		Matrix mProjection;
	} m_transform;

	struct LightBuffer
	{
		Vector3 Direction = { 0.3f, 0.0f, 0.6f }; 
		float pad0;								  
		Vector4 Ambient = { 0.1f,0.1f,0.1f,1.0f };
		Vector4 Diffuse = { 1.0f,1.0f,1.0f,1.0f };
		Vector4 Specular = { 1.0f,1.0f,1.0f,1.0f };
		Vector3 EyePosition;					  
		float pad1;								  
	} m_light;

	struct MarterialBuffer
	{
		Vector4 Ambient = { 1.0f,1.0f,1.0f,1.0f };
		Vector4 Diffuse = { 1.0f,1.0f,1.0f,1.0f };
		Vector4 Specular = { 1.0f,1.0f,1.0f,1.0f };
		float  SpecularPower = 200;
		Vector3 pad0;								  
	} m_material;

	struct UsingBuffer
	{
		// bool ũ�Ⱑ �ٸ�;;
		bool UsingDiffuseMap = true; 
		bool padding0[3];
		bool UsingNormalMap = true;
		bool padding1[3];
		bool UsingSpecularMap = true;
		bool padding2[3];
		bool UsingEmissiveMap = true;
		bool padding3[3];

		bool UsingOpacityMap = true;
		bool padding4[3];

		Vector3 padding5;
	} m_using;

	struct Camera
	{
		Vector3 pos = { 0,300,-500 };
		Vector3 dir = { 0,0,1 };
		Vector3 headDir = { 0,1,0 };
		Matrix viewMatrix;

		float fovY = 1.570796327f;
		float nearZ = 0.1f;
		float farZ = 10000.0f;
		Matrix projMatrix;
	} m_camera;

	friend class Mesh;
	friend class Material;
	friend class Node;
};

