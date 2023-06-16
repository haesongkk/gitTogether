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
	/// Win32 ����
	// ���� Ŭ����
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

	// ���� Ŭ���� ���
	RegisterClass(&wndclass);

	// ���� ����
	m_hWnd = CreateWindow(
		szAppName,
		szAppName,
		WS_OVERLAPPEDWINDOW,
		100, 100, m_ScreenWidth, m_ScreenHeight,
		NULL, NULL, hInstance, NULL);

	if (!m_hWnd) return FALSE;

	// ������ ������ ȭ�鿡 ǥ��
	ShowWindow(m_hWnd, SW_SHOWNORMAL);
	UpdateWindow(m_hWnd);
#pragma endregion

	/// �׷��Ƚ� ���� ����

	// �׷��Ƚ� ���� �ʱ�ȭ
	m_pDREngine = new DRD2DEngine();
	m_pDREngine->Initialize(hInstance, m_hWnd, m_ScreenWidth, m_ScreenHeight);


	/// ������Ʈ �ʱ�ȭ

	// �������� ������ ���ӿ�����Ʈ �ʱ�ȭ

	// 1. ���� ������Ʈ ���ο��� ����/�ʱ�ȭ �ϴ� ���
	m_pTargetObject = new GameObject("TargetObject");
	m_pTargetObject->CreateTransform(CVECTOR2(300, 310), 15.f, CVECTOR2(3.0f, 1.0f));
	m_pTargetObject->CreateCircleRenderer(100.0f);

	m_pMyObject = new CPlayer("MyObject");
	m_pMyObject->CreateTransform(CVECTOR2(400, 400), 0, CVECTOR2(1.f, 1.f));
	m_pMyObject->CreateCircleRenderer(100.0f);


	/*
	// 2. �ܺο��� ���� ��Ҹ� �����ϴ� ���

	//--------------------------------------------------
	// ���ӿ�����Ʈ 1

	// ���ӿ�����Ʈ ����
	m_pTargetObject = new GameObject("TargetObject");

	// ���Ե� ���� ��� �ʱ�ȭ
	Transform* _newTransform = new Transform();
	_newTransform->SetLocalPosition(CVECTOR2(300, 310));
	_newTransform->SetLocalRotation(15.f);
	_newTransform->SetLocalScale(CVECTOR2(3.0f, 1.0f));

	// ������
	CircleRenderer* _newRenderer = new CircleRenderer();
	_newRenderer->m_Radius = 100.0f;

	// ���� ��ҵ��� ���� ������Ʈ�� ����
	m_pTargetObject->AttachTransform(_newTransform);
	m_pTargetObject->AttachRenderer(_newRenderer);

	//--------------------------------------------------
	// ���ӿ�����Ʈ 2

	// ���ӿ�����Ʈ ����
	m_pMyObject = new CPlayer("MyObject");

	Transform* _newTransform2 = new Transform();
	_newTransform2->SetLocalPosition(CVECTOR2(400, 400));
	_newTransform2->SetLocalRotation(0);
	_newTransform2->SetLocalScale(CVECTOR2(1.f, 1.f));

	// ������ ������Ʈ
	CircleRenderer* _newRenderer2 = new CircleRenderer();
	_newRenderer2->m_Radius = 100.0f;

	// ���� ��ҵ��� ���� ������Ʈ�� ����
	m_pMyObject->AttachTransform(_newTransform2);
	m_pMyObject->AttachRenderer(_newRenderer2);
	*/


	///--------------------------------------------------
	/// ���� ������Ʈ ����Ʈ�� �ְ� ������

	// (�ڽ��� �����͸� �θ� �����ͷ� ����.������)
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
	// ������Ʈ�� ������Ʈ�Ѵ�.
	for (GameObject* _pGO : m_GameObjects)
	{
		_pGO->Update();
	}
}

void CGameProcess::RenderAll()
{
	/// �׸��⸦ �غ��Ѵ�.
	m_pDREngine->BeginRender();

	/// ������Ʈ�� TRS�� ������� �ʴ� UI��

	// ���� ��� �׽�Ʈ
	m_pDREngine->SetBrushColor(1.0f, 1.0f, 1.0f, 1.0f);

	m_pDREngine->DrawText(10, 10, "D2D_04_Transform_02 - Transform/Renderer ���� ���� ����");
	m_pDREngine->DrawText(10, 30, "MyObject : ȭ��ǥ - �̵� / SpaceBar - ȸ�� / Enter - ���� / P - WPos(10,10)�����̵�");

	m_pDREngine->DrawText(630, 100, "Status");

	m_pDREngine->DrawText(630, 150, "ElapsedTime : %d", 16);
	m_pDREngine->DrawText(630, 170, "----------------------------");

	m_pDREngine->DrawText(630, 190, m_pMyObject->GetDebugData_Name().c_str());
	m_pDREngine->DrawText(630, 210, m_pMyObject->GetDebugData_LocalPos().c_str());
	m_pDREngine->DrawText(630, 230, m_pMyObject->GetDebugData_WorldPos().c_str());
	m_pDREngine->DrawText(630, 250, m_pMyObject->GetDebugData_Rotation().c_str());
	m_pDREngine->DrawText(630, 270, m_pMyObject->GetDebugData_Scale().c_str());

	m_pDREngine->DrawText(630, 290, "----------------------------");


	// ������Ʈ�� �׸���.
	for (GameObject* _pGO : m_GameObjects)
	{
		_pGO->Render();
	}


	/// �׸��⸦ ������.
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

// �޽��� �ڵ鷯 (���� �ݹ�)
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
