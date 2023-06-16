#include "CGameProcess.h"
#include "GameObject.h"
#include "Transform.h"
#include "Renderer.h"
#include "CircleRenderer.h"
#include "CPlayer.h"

using namespace Traditional;

CGameProcess::CGameProcess()
	: m_hWnd(NULL), m_Msg(), m_pDREngine(nullptr),
	m_pTargetObject(nullptr), m_pMyObject(nullptr), m_pMyChild(nullptr)
{

}

CGameProcess::~CGameProcess()
{

}

HRESULT CGameProcess::Initialize(HINSTANCE hInstance)
{
#pragma region _WINDOWINIT_
	/// Win32 관련
	// 윈도 클래스
	char szAppName[] = "GameAcademy 2021 Direct2D Demo";
	WNDCLASS wndclass;

	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = CGameProcess::WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = szAppName;

	// 윈도 클래스 등록
	RegisterClass(&wndclass);

	// 윈도 생성
	m_hWnd = CreateWindow(
		szAppName,
		szAppName,
		WS_OVERLAPPEDWINDOW,
		100, 100, m_ScreenWidth, m_ScreenHeight,
		NULL, NULL, hInstance, NULL);

	if (!m_hWnd) return FALSE;

	// 생성된 윈도를 화면에 표시
	ShowWindow(m_hWnd, SW_SHOWNORMAL);
	UpdateWindow(m_hWnd);
#pragma endregion

	/// 그래픽스 엔진 관련

	// 그래픽스 엔진 초기화
	m_pDREngine = new DRD2DEngine();
	m_pDREngine->Initialize(hInstance, m_hWnd, m_ScreenWidth, m_ScreenHeight);


	/// 오브젝트 초기화

	// 전통적인 형태의 게임오브젝트 초기화

	// 1. 게임 오브젝트 내부에서 생성/초기화 하는 경우
	m_pTargetObject = new GameObject("TargetObject");
	m_pTargetObject->CreateTransform(CVECTOR2(300, 310), 15.f, CVECTOR2(3.0f, 1.0f));
	m_pTargetObject->CreateCircleRenderer(100.0f);

	m_pMyObject = new CPlayer("MyObject");
	m_pMyObject->CreateTransform(CVECTOR2(400, 400), 0, CVECTOR2(1.f, 1.f));
	m_pMyObject->CreateCircleRenderer(100.0f);


	/*
	// 2. 외부에서 구성 요소를 생성하는 경우

	//--------------------------------------------------
	// 게임오브젝트 1

	// 게임오브젝트 생성
	m_pTargetObject = new GameObject("TargetObject");

	// 포함된 구성 요소 초기화
	Transform* _newTransform = new Transform();
	_newTransform->SetLocalPosition(CVECTOR2(300, 310));
	_newTransform->SetLocalRotation(15.f);
	_newTransform->SetLocalScale(CVECTOR2(3.0f, 1.0f));

	// 랜더러
	CircleRenderer* _newRenderer = new CircleRenderer();
	_newRenderer->m_Radius = 100.0f;

	// 구성 요소들을 게임 오브젝트에 포함
	m_pTargetObject->AttachTransform(_newTransform);
	m_pTargetObject->AttachRenderer(_newRenderer);

	//--------------------------------------------------
	// 게임오브젝트 2

	// 게임오브젝트 생성
	m_pMyObject = new CPlayer("MyObject");

	Transform* _newTransform2 = new Transform();
	_newTransform2->SetLocalPosition(CVECTOR2(400, 400));
	_newTransform2->SetLocalRotation(0);
	_newTransform2->SetLocalScale(CVECTOR2(1.f, 1.f));

	// 랜더러 컴포넌트
	CircleRenderer* _newRenderer2 = new CircleRenderer();
	_newRenderer2->m_Radius = 100.0f;

	// 구성 요소들을 게임 오브젝트에 포함
	m_pMyObject->AttachTransform(_newTransform2);
	m_pMyObject->AttachRenderer(_newRenderer2);
	*/


	///--------------------------------------------------
	/// 게임 오브젝트 리스트에 넣고 관리함

	// (자식의 포인터를 부모 포인터로 받음.다형성)
	m_GameObjects.push_back(m_pTargetObject);
	m_GameObjects.push_back(m_pMyObject);

	return S_OK;
}

void CGameProcess::Loop()
{
	while (true)
	{
		if (PeekMessage(&m_Msg, NULL, 0, 0, PM_REMOVE))
		{
			if (m_Msg.message == WM_QUIT) break;

			DispatchMessage(&m_Msg);
		}
		else
		{
			UpdateAll();
			RenderAll();
		}
	}
}

void CGameProcess::UpdateAll()
{
	// 오브젝트를 업데이트한다.
	for (GameObject* _pGO : m_GameObjects)
	{
		_pGO->Update();
	}
}

void CGameProcess::RenderAll()
{
	/// 그리기를 준비한다.
	m_pDREngine->BeginRender();

	/// 오브젝트별 TRS를 영향받지 않는 UI등

	// 문자 출력 테스트
	m_pDREngine->SetBrushColor(1.0f, 1.0f, 1.0f, 1.0f);

	m_pDREngine->DrawText(10, 10, "D2D_04_Transform_02 - Transform/Renderer 포함 구조 예제");
	m_pDREngine->DrawText(10, 30, "MyObject : 화살표 - 이동 / SpaceBar - 회전 / Enter - 정지 / P - WPos(10,10)강제이동");

	m_pDREngine->DrawText(630, 100, "Status");

	m_pDREngine->DrawText(630, 150, "ElapsedTime : %d", 16);
	m_pDREngine->DrawText(630, 170, "----------------------------");

	m_pDREngine->DrawText(630, 190, m_pMyObject->GetDebugData_Name().c_str());
	m_pDREngine->DrawText(630, 210, m_pMyObject->GetDebugData_LocalPos().c_str());
	m_pDREngine->DrawText(630, 230, m_pMyObject->GetDebugData_WorldPos().c_str());
	m_pDREngine->DrawText(630, 250, m_pMyObject->GetDebugData_Rotation().c_str());
	m_pDREngine->DrawText(630, 270, m_pMyObject->GetDebugData_Scale().c_str());

	m_pDREngine->DrawText(630, 290, "----------------------------");


	// 오브젝트를 그린다.
	for (GameObject* _pGO : m_GameObjects)
	{
		_pGO->Render();
	}


	/// 그리기를 끝낸다.
	m_pDREngine->EndRender();
}

void CGameProcess::Finalize()
{
	for (GameObject* go : m_GameObjects)
	{
		if (go != nullptr)
		{
			delete go;
		}
	}

	m_pDREngine->Finalize();

	delete m_pDREngine;
}

// 메시지 핸들러 (윈도 콜백)
LRESULT CALLBACK CGameProcess::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC			hdc;
	PAINTSTRUCT ps;

	switch (message)
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}
