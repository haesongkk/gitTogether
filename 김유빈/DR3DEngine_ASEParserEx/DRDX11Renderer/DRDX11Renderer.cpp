#include "DRDX11Renderer.h"

#pragma comment( lib, "d3d11.lib" )
#pragma comment( lib, "dxgi.lib" )	// adapter info

#include <d3dcompiler.h>	// fx compile

//#include <DirectXColors.h>
#include "DXTKFont.h"

//#include <DirectXMath.h>
#include "SimpleMath.h"

#include "DRCamera.h"
#include "Axis.h"
#include "Grid.h"
#include "Skull.h"
#include "Box.h"
#include "Crate.h"
#include "Model.h"

#ifdef _DEBUG
#pragma comment( lib, "../Lib/Effects11d.lib" )
#else
#pragma comment( lib, "../Lib/Effects11.lib" )
#endif

DRDX11Renderer::DRDX11Renderer()
	: mMainWndCaption(L"D3D11 Application"),
	md3dDriverType(D3D_DRIVER_TYPE_HARDWARE),
	mClientWidth(800),
	mClientHeight(600),
	mEnable4xMsaa(false),
	mhMainWnd(0),
	mAppPaused(false),
	mMinimized(false),
	mMaximized(false),
	m4xMsaaQuality(0),

	md3dDevice(0),
	md3dImmediateContext(0),
	mSwapChain(0),
	mDepthStencilBuffer(0),
	mRenderTargetView(0),
	mDepthStencilView(0),
	m_pFont(nullptr), m_deltaTime(0),

	mWireframeRS(0), mSolidRS(0), NormalDSS(nullptr),
	m_WorldAxes(nullptr), m_Grid(nullptr),
	m_LitSkull(nullptr), m_TestBox(nullptr), m_TestCrate(nullptr),
	m_pASEParser(nullptr), m_Genji(nullptr)
{
	ZeroMemory(&mScreenViewport, sizeof(D3D11_VIEWPORT));

	m_pFont = new DXTKFont();
	m_pCamera = new DRCamera();
	//m_pCamera->SetPosition(8.0f, 8.0f, -8.0f);
	m_pCamera->LookAt(XMFLOAT3(50.0f, 100.0f, -300.0f), XMFLOAT3(0, 100, 0), XMFLOAT3(0, 1.0f, 0));

	//m_pASEParser = new CASEParser();
	//m_pASEParser->Init();
	//m_pASEParser->Load((LPSTR)"../ASEFile/genji_max.ASE");
	//m_pASEParser->Load((LPSTR)"../ASEFile/03IK-Joe_onlymesh.ASE");
	//m_pASEParser->Load((LPSTR)"../ASEFile/P38.ASE");
	//m_pASEParser->Load((LPSTR)"../ASEFile/teapot.ASE");


	///m_pASEParser->Translate_Optimize(m_pASEParser->GetMesh(0));
	//for (int i = 0; i < m_pASEParser->GetMeshNum(); i++)
	//{
	//	m_pASEParser->ConvertAll(m_pASEParser->GetMesh(i));
	//}

}

/// 릴리즈/삭제는 할당/생성의 반대 순서로
DRDX11Renderer::~DRDX11Renderer()
{
	InputLayouts::DestroyAll();
	Effects::DestroyAll();

	SafeDelete(m_Genji);
	SafeDelete(m_pASEParser)

	SafeDelete(m_TestCrate);
	SafeDelete(m_TestBox);
	SafeDelete(m_LitSkull);

	SafeDelete(m_Grid);
	SafeDelete(m_WorldAxes);

	SafeDelete(m_pCamera);
	SafeDelete(m_pFont);

	ReleaseCOM(NormalDSS);
	ReleaseCOM(mWireframeRS);
	ReleaseCOM(mSolidRS);

	ReleaseCOM(mRenderTargetView);
	ReleaseCOM(mDepthStencilView);
	ReleaseCOM(mSwapChain);
	ReleaseCOM(mDepthStencilBuffer);

	// Restore all default settings.
	if (md3dImmediateContext)
	{
		md3dImmediateContext->ClearState();
	}

	ReleaseCOM(md3dImmediateContext);
	ReleaseCOM(md3dDevice);
}

bool DRDX11Renderer::Initialize(int hinst, int hWnd, int screenWidth, int screenHeight)
{
	mhMainWnd = (HWND)hWnd;
	mClientWidth = screenWidth;
	mClientHeight = screenHeight;

	/// 디버그 빌드인 경우 DX11 디버그 모드

	UINT createDeviceFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)  
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	/// D3디바이스와 디바이스 컨텍스트를 생성한다.

	// 피쳐 레벨
	///D3D_FEATURE_LEVEL featureLevel;
	HRESULT hr = D3D11CreateDevice(
		0,                 // default adapter
		md3dDriverType,
		0,                 // no software device
		createDeviceFlags,
		0, 0,              // default feature level array
		D3D11_SDK_VERSION,
		&md3dDevice,
		&featureLevel,
		&md3dImmediateContext);

	if (FAILED(hr))
	{
		MessageBox(0, L"D3D11CreateDevice Failed.", 0, 0);
		return false;
	}

	if (featureLevel != D3D_FEATURE_LEVEL_11_0)
	{
		MessageBox(0, L"Direct3D Feature Level 11 unsupported.", 0, 0);
		return false;
	}

	/// 멀티샘플링 몇단계까지 되는지 체크
	// Check 4X MSAA quality support for our back buffer format.
	// All Direct3D 11 capable devices support 4X MSAA for all render 
	// target formats, so we only need to check quality support.

	HR(md3dDevice->CheckMultisampleQualityLevels(
		DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m4xMsaaQuality));
	assert(m4xMsaaQuality > 0);

	// Fill out a DXGI_SWAP_CHAIN_DESC to describe our swap chain.

	DXGI_SWAP_CHAIN_DESC sd;
	sd.BufferDesc.Width = mClientWidth;
	sd.BufferDesc.Height = mClientHeight;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// Use 4X MSAA? 
	if (mEnable4xMsaa)
	{
		sd.SampleDesc.Count = 4;
		sd.SampleDesc.Quality = m4xMsaaQuality - 1;
	}
	// No MSAA
	else
	{
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
	}

	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = mhMainWnd;
	sd.Windowed = true;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;

	// To correctly create the swap chain, we must use the IDXGIFactory that was
	// used to create the device.  If we tried to use a different IDXGIFactory instance
	// (by calling CreateDXGIFactory), we get an error: "IDXGIFactory::CreateSwapChain: 
	// This function is being called with a device from a different IDXGIFactory."

	// D3디바이스로부터 dxgi디바이스를 얻어온다.
	IDXGIDevice* dxgiDevice = 0;
	HR(md3dDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice));

	// dxgi디바이스로부터 dxgi어댑터를 얻어온다.
	IDXGIAdapter* dxgiAdapter = 0;
	HR(dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter));

	// dxgi어댑터로부터 dxgi팩토리를 얻어온다.
	IDXGIFactory* dxgiFactory = 0;
	HR(dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory));

	// 드디어 스왑체인을 생성한다.
	HR(dxgiFactory->CreateSwapChain(md3dDevice, &sd, &mSwapChain));

	ReleaseCOM(dxgiDevice);
	ReleaseCOM(dxgiAdapter);
	ReleaseCOM(dxgiFactory);


	// The remaining steps that need to be carried out for d3d creation
	// also need to be executed every time the window is resized.  So
	// just call the OnResize method here to avoid code duplication.

	// 그렇다는데 사실 여기서 하는것도 많다.
	OnResize();



	// Effects
	/// 입력 배치(InputLayout)가 쉐이더 시그니처(sementics/의미소)에 의존하기 때문에, 이펙트를 먼저 초기화해야 함.
	// Must init Effects first since InputLayouts depend on shader signatures.
	Effects::InitAll(md3dDevice);
	InputLayouts::InitAll(md3dDevice);


	// Render State
	CreateRenderStates();


	// 폰트 생성
	m_pFont->Create(md3dDevice, mSolidRS, NormalDSS);

	// Axis
	m_WorldAxes = new Axis(md3dDevice, md3dImmediateContext, mWireframeRS);
	m_WorldAxes->Initialize();

	m_Grid = new Grid(md3dDevice, md3dImmediateContext, mWireframeRS);
	m_Grid->Initialize();

	// Mesh Object Test
	m_LitSkull = new Skull(md3dDevice, md3dImmediateContext, mSolidRS);
	m_LitSkull->Initialize();

	// Box 예시
	m_TestBox = new Box(md3dDevice, md3dImmediateContext, mSolidRS);
	m_TestBox->Initialize();

	// Crate 예시
	m_TestCrate = new Crate(md3dDevice, md3dImmediateContext, mSolidRS);
	m_TestCrate->Initialize();

	// 파싱 파일에 따른 모델(여러개의 메시를 갖는) 생성
	m_Genji = new Model(md3dDevice, md3dImmediateContext, mSolidRS);
	m_Genji->Initialize();

	/// 위의 빌드 기하버퍼 대신에 이것을 써서 ASE 파일 로드
	//m_Genji->LoadGeomerty(m_pASEParser->GetMesh(1));
	//for (int i = 0; i < m_pASEParser->GetMeshNum(); i++)
	//{
	//	MeshObject* mesh = new MeshObject(md3dDevice, md3dImmediateContext, mSolidRS);
	//	mesh->Initialize();
	//	mesh->LoadGeomerty(m_pASEParser->GetMesh(i));

	//	m_MeshList.push_back(mesh);
	//}

	/// 요기
	//for (int i = 0; i < m_pASEParser->GetMeshNum(); i++)
	//{
	//	if (m_MeshList[i])
	//}

	// 어댑터 정보를 얻는다.
	GetAdapterInfo();

	return true;
}

void DRDX11Renderer::Update(float deltaTime)
{
	m_deltaTime = deltaTime;

	// 카메라
	if (GetAsyncKeyState('W') & 0x8000)
		m_pCamera->Walk(50.0f * deltaTime);

	if (GetAsyncKeyState('S') & 0x8000)
		m_pCamera->Walk(-50.0f * deltaTime);

	if (GetAsyncKeyState('A') & 0x8000)
		m_pCamera->Strafe(-50.0f * deltaTime);

	if (GetAsyncKeyState('D') & 0x8000)
		m_pCamera->Strafe(50.0f * deltaTime);

	if (GetAsyncKeyState('Q') & 0x8000)
		m_pCamera->WorldUpDown(-50.0f * deltaTime);

	if (GetAsyncKeyState('E') & 0x8000)
		m_pCamera->WorldUpDown(50.0f * deltaTime);

	// view TM을 만든다.
	m_pCamera->UpdateViewMatrix();

	// Axis
	m_WorldAxes->Update(XMMatrixIdentity(), m_pCamera->View(), m_pCamera->Proj());
	m_Grid->Update(XMMatrixIdentity(), m_pCamera->View(), m_pCamera->Proj());

	// Mesh Object
	m_LitSkull->Update(m_pCamera);

	// Box예시
	m_TestBox->Update(XMMatrixIdentity(), m_pCamera->View(), m_pCamera->Proj());

	// Crate 예시
	m_TestCrate->Update(m_pCamera);

	m_Genji->Update(m_pCamera, deltaTime);

	//for (int i = 0; i < m_pASEParser->GetMeshNum(); i++)
	//{
	//	m_MeshList[i]->Update(m_pCamera);
	//}
}

void DRDX11Renderer::OnMouseDown(int x, int y)
{
	mLastMousePos.x = x;
	mLastMousePos.y = y;

	SetCapture(mhMainWnd);
}

void DRDX11Renderer::OnMouseUp(int x, int y)
{
	ReleaseCapture();
}

void DRDX11Renderer::OnMouseMove(int btnState, int x, int y)
{
	if ((btnState & MK_RBUTTON) != 0)
	{
		// Make each pixel correspond to a quarter of a degree.
		float dx = XMConvertToRadians(0.25f * static_cast<float>(x - mLastMousePos.x));
		float dy = XMConvertToRadians(0.25f * static_cast<float>(y - mLastMousePos.y));

		m_pCamera->Pitch(dy);
		m_pCamera->RotateY(dx);
	}

	mLastMousePos.x = x;
	mLastMousePos.y = y;
}

/// <summary>
/// 렌더링 하기 전
/// </summary>
void DRDX11Renderer::BeginRender()
{
	assert(md3dImmediateContext);

	// 랜더타겟 뷰를 클리어한다.
	md3dImmediateContext->ClearRenderTargetView(mRenderTargetView, reinterpret_cast<const float*>(&DRColors::DeepDarkGray));

	// 뎁스스탠실 뷰를 클리어한다.
	md3dImmediateContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

// BeginRender()와 EndRender() 사이에 호출 될 것으로 기대하는 테스트용 함수.
void DRDX11Renderer::Draw_Test()
{
	///----------------------------------------------------------------------------------------------------
	/// 오브젝트들을 그린다. (Draw Primitive)
	md3dImmediateContext->OMSetDepthStencilState(NormalDSS, 0);

	// 그리드와 축을 그린다.
	m_Grid->Render();
	m_WorldAxes->Render();
	
	//m_LitSkull->Render();	// 해골을 그린다.
	///m_TestBox->Render();	// 박스 그리기 예시
	///m_TestCrate->Render();	// 텍스쳐 예시

	// ASE Parser 예시
	m_Genji->Render();

	//for (int i = 0; i < m_pASEParser->GetMeshNum(); i++)
	//{
	//	m_MeshList[i]->Render();
	//}

	///----------------------------------------------------------------------------------------------------
	/// 폰트 등 UI를 그린다. 위의 DrawPrimitive보다 뒤에 해야 한다.

	/// 폰트 라이브러리 테스트
	///m_pFont->DrawTest();

	// 폰트 뎁스 문제
	///md3dImmediateContext->OMSetDepthStencilState(EqualsDSS, 0);

	//DirectX::XMFLOAT4 test(0, 1.f, 1.f, 1.f);					// 안된다.
	//DirectX::FXMVECTOR test2 = DirectX::XMVECTOR();			// 직접적으로 초기화 할 방법이 마땅치 않다.
	DirectX::SimpleMath::Vector4 test3(1.f, 1.f, 0.f, 1.f);		// 드럽네..
	DirectX::SimpleMath::Vector4 _white(1.f, 1.f, 1.f, 1.f);	// white

	m_pFont->DrawTextColor(200, 100, test3, (TCHAR*)L"한글과 버퍼가 잘 작동하는가? %d / %f / %s", 79, 3.14f, L"노란색글씨 똠방각하 펲시 콜라 뾸뾸");

	// 갱신주기
	static float _addedTime = 0;
	static float _FPS = 0;
	static float _deltaTimeMS = 0;

	// 갱신주기는 0.2초
	if (0.2f < _addedTime)
	{
		_FPS = (1.0f / m_deltaTime);
		_deltaTimeMS = m_deltaTime * 1000.0f;

		_addedTime = 0;
	}

	_addedTime += (m_deltaTime);

	// FPS, deltaTime을 그린다.
	m_pFont->DrawTextColor(0, 10, _white, (TCHAR*)L"FPS : %.2f", _FPS);
	m_pFont->DrawTextColor(0, 24, _white, (TCHAR*)L"DTime : %.4f ms", _deltaTimeMS);

	// 피쳐레벨, 어댑터 등의 상태를 그린다.
	this->Draw_Status();


	// Restore default.
	md3dImmediateContext->RSSetState(0);
}

/// <summary>
/// 창이 리사이즈 됐을 때 처리해야 할 것들
/// </summary>
void DRDX11Renderer::OnResize()
{
	assert(md3dImmediateContext);
	assert(md3dDevice);
	assert(mSwapChain);

	// Release the old views, as they hold references to the buffers we
	// will be destroying.  Also release the old depth/stencil buffer.

	ReleaseCOM(mRenderTargetView);
	ReleaseCOM(mDepthStencilView);
	ReleaseCOM(mDepthStencilBuffer);


	// Resize the swap chain and recreate the render target view.

	HR(mSwapChain->ResizeBuffers(1, mClientWidth, mClientHeight, DXGI_FORMAT_R8G8B8A8_UNORM, 0));
	ID3D11Texture2D* backBuffer;
	HR(mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer)));
	HR(md3dDevice->CreateRenderTargetView(backBuffer, 0, &mRenderTargetView));
	ReleaseCOM(backBuffer);

	// Create the depth/stencil buffer and view.

	D3D11_TEXTURE2D_DESC depthStencilDesc;

	depthStencilDesc.Width = mClientWidth;
	depthStencilDesc.Height = mClientHeight;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	// Use 4X MSAA? --must match swap chain MSAA values.
	if (mEnable4xMsaa)
	{
		depthStencilDesc.SampleDesc.Count = 4;
		depthStencilDesc.SampleDesc.Quality = m4xMsaaQuality - 1;
	}
	// No MSAA
	else
	{
		depthStencilDesc.SampleDesc.Count = 1;
		depthStencilDesc.SampleDesc.Quality = 0;
	}

	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	HR(md3dDevice->CreateTexture2D(&depthStencilDesc, 0, &mDepthStencilBuffer));
	HR(md3dDevice->CreateDepthStencilView(mDepthStencilBuffer, 0, &mDepthStencilView));


	// Bind the render target view and depth/stencil view to the pipeline.
	/// 렌더타겟뷰, 뎁스/스탠실뷰를 파이프라인에 바인딩한다.
	md3dImmediateContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);


	// Set the viewport transform.
	/// 뷰포트 변환을 셋팅한다.
	mScreenViewport.TopLeftX = 0;
	mScreenViewport.TopLeftY = 0;
	mScreenViewport.Width = static_cast<float>(mClientWidth);
	mScreenViewport.Height = static_cast<float>(mClientHeight);
	mScreenViewport.MinDepth = 0.0f;
	mScreenViewport.MaxDepth = 1.0f;

	md3dImmediateContext->RSSetViewports(1, &mScreenViewport);

	// The window resized, so update the aspect ratio and recompute the projection matrix.
	/// 창의 크기가 변했으므로, 종횡비를 업데이트하고 투영 행렬을 재계산한다.
	m_pCamera->SetLens(0.25f * MathHelper::Pi, GetAspectRatio(), 1.0f, 1000.0f);
}

/// <summary>
/// 렌더링 마무리. 그릴 것 다 그린 상태에서 Present
/// </summary>
void DRDX11Renderer::EndRender()
{
	// Set per frame constants.
	///Effects::BasicFX->SetDirLights(mDirLights);
	///Effects::BasicFX->SetEyePosW(m_pCamera->GetPosition());

	assert(mSwapChain);

	// 프리젠트 꼭 해줘야 한다.
	HR(mSwapChain->Present(0, 0));
}

/// <summary>
/// 랜더러를 종료한다.
/// </summary>
void DRDX11Renderer::Finalize()
{
	// DX의 자원 해제등은 소멸자라는 명시적인 타이밍이 있다.
	// 여기는 그 외 자원들 (스크립트, 객체간의 레퍼런스 해제 등)을 처리한다.
}

bool DRDX11Renderer::IsValidDevice()
{
	return (md3dDevice != nullptr);
}

void DRDX11Renderer::SetClientSize(int width, int height)
{
	mClientWidth = width;
	mClientHeight = height;
}

void DRDX11Renderer::SetWinMinMax(bool isMinimized, bool isMaxmized)
{
	mMinimized = isMinimized;
	mMaximized = isMaxmized;
}

/// <summary>
/// 피쳐레벨, 해상도, 어댑터 등 이것저것 모두 출력
/// 엔진 스펙으로 놓자. 어디까지나 학생들은 엔진을 만드는 것이므로..
/// </summary>
void DRDX11Renderer::Draw_Status()
{
	DirectX::SimpleMath::Vector4 _white(1.f, 1.f, 1.f, 1.f);	// white

	// 피쳐레벨
	int _yPos = 50;
	m_pFont->DrawTextColor(0, _yPos, _white, (TCHAR*)L"Feature Level : %x", featureLevel);

	// 어댑터 정보
	m_pFont->DrawTextColor(0, _yPos += 14, _white, (TCHAR*)L"Description: %s", m_AdapterDesc1.Description);
	m_pFont->DrawTextColor(0, _yPos += 14, _white, (TCHAR*)L"VendorID: %u", m_AdapterDesc1.VendorId);
	m_pFont->DrawTextColor(0, _yPos += 14, _white, (TCHAR*)L"DeviceID: %u", m_AdapterDesc1.DeviceId);
	m_pFont->DrawTextColor(0, _yPos += 14, _white, (TCHAR*)L"SubSysID: %u", m_AdapterDesc1.SubSysId);
	m_pFont->DrawTextColor(0, _yPos += 14, _white, (TCHAR*)L"Revision: %u", m_AdapterDesc1.Revision);
	m_pFont->DrawTextColor(0, _yPos += 14, _white, (TCHAR*)L"VideoMemory: %lu MB", m_AdapterDesc1.DedicatedVideoMemory / 1024 / 1024);
	m_pFont->DrawTextColor(0, _yPos += 14, _white, (TCHAR*)L"SystemMemory: %lu MB", m_AdapterDesc1.DedicatedSystemMemory / 1024 / 1024);
	m_pFont->DrawTextColor(0, _yPos += 14, _white, (TCHAR*)L"SharedSysMemory: %lu MB", m_AdapterDesc1.SharedSystemMemory / 1024 / 1024);
	m_pFont->DrawTextColor(0, _yPos += 14, _white, (TCHAR*)L"AdpaterLuid: %u.%d", m_AdapterDesc1.AdapterLuid.HighPart, m_AdapterDesc1.AdapterLuid.LowPart);

	// 클라이언트 영역의 해상도
	m_pFont->DrawTextColor(0, _yPos += 28, _white, (TCHAR*)L"Client Resolution: %dx%d", mClientWidth, mClientHeight);

	// 카메라 정보
	m_pFont->DrawTextColor(0, _yPos += 28, _white, (TCHAR*)L"Camera Pos : %.2f / %.2f / %.2f", m_pCamera->GetPosition().x, m_pCamera->GetPosition().y, m_pCamera->GetPosition().z);
}

float DRDX11Renderer::GetAspectRatio() const
{
	return static_cast<float>(mClientWidth) / mClientHeight;
}

void DRDX11Renderer::CreateRenderStates()
{
	// Render State 중 Rasterizer State
	D3D11_RASTERIZER_DESC solidDesc;
	ZeroMemory(&solidDesc, sizeof(D3D11_RASTERIZER_DESC));
	solidDesc.FillMode = D3D11_FILL_SOLID;
	solidDesc.CullMode = D3D11_CULL_BACK;
	solidDesc.FrontCounterClockwise = false;
	solidDesc.DepthClipEnable = true;

	HR(md3dDevice->CreateRasterizerState(&solidDesc, &mSolidRS));


	D3D11_RASTERIZER_DESC wireframeDesc;
	ZeroMemory(&wireframeDesc, sizeof(D3D11_RASTERIZER_DESC));
	wireframeDesc.FillMode = D3D11_FILL_WIREFRAME;
	wireframeDesc.CullMode = D3D11_CULL_BACK;
	wireframeDesc.FrontCounterClockwise = false;
	wireframeDesc.DepthClipEnable = true;

	HR(md3dDevice->CreateRasterizerState(&wireframeDesc, &mWireframeRS));


	//
	// 폰트용 DSS
	//
	D3D11_DEPTH_STENCIL_DESC equalsDesc;
	ZeroMemory(&equalsDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	equalsDesc.DepthEnable = true;
	equalsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;		// 깊이버퍼에 쓰기는 한다
	equalsDesc.DepthFunc = D3D11_COMPARISON_LESS;

	HR(md3dDevice->CreateDepthStencilState(&equalsDesc, &NormalDSS));
}

// 디바이스/GPU 정보 획득 <방법 #1> 
// DXGI 1.1 / DXGI Factory 사용 / DX11 이상시 권장, DXGI 1.0 과 1.1 혼용금지. DXGI 1.3 은 Windows 8.1 이상필수.

// DXGI 1.1 대응
HRESULT DRDX11Renderer::GetAdapterInfo()
{
	// DXGI버전별로 다름
	IDXGIAdapter1* pAdapter;
	IDXGIFactory1* pFactory = NULL;

	HRESULT hr = S_OK;

	// DXGIFactory 개체 생성(DXGI.lib 필요)
	if (FAILED(CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)&pFactory)))
	{
		return E_FAIL;
	}
	
	hr = pFactory->EnumAdapters1(0, &pAdapter);
		
	if (hr != DXGI_ERROR_NOT_FOUND)
	{
		pAdapter->GetDesc1(&m_AdapterDesc1);		 
	}

	SAFE_RELEASE(pAdapter);
	SAFE_RELEASE(pFactory);

	return S_OK;
}
