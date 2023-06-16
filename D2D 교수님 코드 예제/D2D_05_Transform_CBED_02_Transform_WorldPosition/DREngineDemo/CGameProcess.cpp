#include "CGameProcess.h"
#include "GameObject.h"
#include "Component.h"
#include "Transform.h"
#include "Renderer.h"
#include "PrimitiveRenderer2D.h"
#include "CPlayer.h"
#include "CChildPlayer.h"

using namespace CBEngine;

CGameProcess::CGameProcess()
	: m_hWnd(NULL), m_Msg(), m_pDREngine(nullptr),
	m_KeyUp(false), m_KeyDown(false), m_KeyLeft(false), m_KeyRight(false),
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
	char szAppName[] = "GameAcademy 2019 Direct2D Demo";
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

	/// ������Ʈ ����� ���� ������

	
	/// ���ӿ�����Ʈ 1

	// ��ƼƼ ����
	m_pTargetObject = new CBEngine::GameObject("TargetObject");

	// ��ȯ ������Ʈ
	Transform* _newTransform = new Transform();
	_newTransform->SetLocalPosition(CVECTOR2(300, 310));
	_newTransform->SetLocalRotation(15.f);
	_newTransform->SetLocalScale(CVECTOR2(3.0f, 1.0f));

	/// TM ����ȯ �׽�Ʈ
	//_newTransform->MakeTransformMatrix();
	//CVECTOR2 _position = _newTransform->GetPositionFromTM(_newTransform->GetTransformMatrix());
	//float _angle = _newTransform->GetRotationFromTM(_newTransform->GetTransformMatrix());
	//CVECTOR2 _scale = _newTransform->GetScaleFromTM(_newTransform->GetTransformMatrix());


	
	// ������ ������Ʈ
	PrimitiveRenderer2D* _newRenderer = new PrimitiveRenderer2D();
	_newRenderer->primitiveType = eCollisionType::eCC;
	_newRenderer->m_Radius = 100.0f;

	// ������Ʈ�� ���ӿ�����Ʈ(��ƼƼ)�� Add
	m_pTargetObject->AddComponent<Transform*>(_newTransform);
	m_pTargetObject->AddComponent<PrimitiveRenderer2D*>(_newRenderer);



	/// ���ӿ�����Ʈ 2
	m_pMyObject = new CBEngine::GameObject("MyObject");

	Transform* _newTransform2 = new Transform();
	_newTransform2->SetLocalPosition(CVECTOR2(400, 400));
	_newTransform2->SetLocalRotation(0);
	_newTransform2->SetLocalScale(CVECTOR2(1.f, 1.f));

	// ������ ������Ʈ
	PrimitiveRenderer2D* _newRenderer2 = new PrimitiveRenderer2D();
	_newRenderer2->primitiveType = eCollisionType::eCC;
	_newRenderer2->m_Radius = 100.0f;

	// ������ ���� �ڵ鸵�� ������Ʈ
	CPlayer* _myPlayer = new CPlayer();

	// ������Ʈ�� ���ӿ�����Ʈ(��ƼƼ)�� Add
	m_pMyObject->AddComponent<Transform*>(_newTransform2);
	m_pMyObject->AddComponent<PrimitiveRenderer2D*>(_newRenderer2);
	m_pMyObject->AddComponent(_myPlayer);


	/// ���ӿ�����Ʈ 3 (2�� Child)
	m_pMyChild = new CBEngine::GameObject("MyChildObject");

	Transform* _newTransform3 = new Transform();
	_newTransform3->SetLocalPosition(CVECTOR2(50, 50));	// ������ǥ���̴�
	_newTransform3->SetLocalRotation(-45.f);				// ��������
	_newTransform3->SetLocalScale(CVECTOR2(1.f, 1.f));

	// ������Ʈ2�� �θ�-�ڽ� ����� �������ش�.
	//_newTransform3->SetParent(_newTransform2);

	// ������ ������Ʈ
	PrimitiveRenderer2D* _newRenderer3 = new PrimitiveRenderer2D();
	_newRenderer3->primitiveType = eCollisionType::eCC;
	_newRenderer3->m_Radius = 30.0f;


	// ������ ���� �ڵ鸵�� ������Ʈ
	CChildPlayer* _childPlayer = new CChildPlayer();

	// ������Ʈ�� ���ӿ�����Ʈ(��ƼƼ)�� Add
	m_pMyChild->AddComponent<Transform*>(_newTransform3);
	m_pMyChild->AddComponent<PrimitiveRenderer2D*>(_newRenderer3);
	m_pMyChild->AddComponent<CChildPlayer*>(_childPlayer);

	//
	//m_pMyChild->GetComponent<Transform>()->SetParent(m_pMyObject->GetComponent<Transform>());
	m_pMyChild->GetComponent2<Transform*>()->SetParent(m_pMyObject->GetComponent<Transform>());

	// ������Ʈ���� ������Ʈ�� ȣ��
	Transform* _componentGetFromComponent1 = _childPlayer->GetComponent<Transform>();
	Transform* _componentGetFromComponent2 = _childPlayer->GetComponent2<Transform*>();



	/// GetComponent �׽�Ʈ
	//Transform* _componentFinded = m_pTargetObject->GetComponent<Transform>();	// ���ø� ������ �����Ͱ� �ƴ�	
	//Transform* _componentFinded2 = m_pTargetObject->GetComponent2<Transform*>();// ������ ��������
	//Renderer* _renderer = m_pTargetObject->GetComponent2<Renderer*>();

	/// ���� ������Ʈ ����Ʈ�� �ְ� ������
	m_GameObjects.push_back(m_pTargetObject);
	m_GameObjects.push_back(m_pMyObject);
	m_GameObjects.push_back(m_pMyChild);



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
	// Ű �Է�
	if (GetAsyncKeyState(VK_UP))		m_KeyUp = true;		else m_KeyUp = false;
	if (GetAsyncKeyState(VK_DOWN))		m_KeyDown = true;	else m_KeyDown = false;
	if (GetAsyncKeyState(VK_RIGHT))		m_KeyRight = true;	else m_KeyRight = false;
	if (GetAsyncKeyState(VK_LEFT))		m_KeyLeft = true;	else m_KeyLeft = false;

	if (GetAsyncKeyState(VK_SPACE))
	{
	}

	if (GetAsyncKeyState(VK_RETURN))
	{
	}


	// ������Ʈ �̵�
	if (m_KeyUp == true)
	{

	}

	if (m_KeyDown == true)
	{

	}

	if (m_KeyRight == true)
	{

	}

	if (m_KeyLeft == true)
	{

	}


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

	m_pDREngine->DrawText(10, 10, "�ѱ��ϰ������A1c");
	m_pDREngine->DrawText(10, 30, "MyObject : ȭ��ǥ - �̵� / SpaceBar - ȸ�� / Enter - ���� / P - WPos(10,10)�����̵�");
	m_pDREngine->DrawText(10, 50, "MyChild : WSAD - �̵� / XC - ȸ�� / Z - ����");


	m_pDREngine->DrawText(630, 100, "Status");

	m_pDREngine->DrawText(630, 150, "ElapsedTime : %d", 16);
	m_pDREngine->DrawText(630, 170, "----------------------------");

	CPlayer* _pPlayer = m_pMyObject->GetComponent2<CPlayer*>();

	//m_pDREngine->DrawText(630, 190, "%s Pos : %d / %d", m_pMyObject->GetName().c_str(), 0, 0);// m_pMyObject->GetComponent2<Transform>());
	m_pDREngine->DrawText(630, 190, _pPlayer->GetDebugData_Name().c_str());
	m_pDREngine->DrawText(630, 210, _pPlayer->GetDebugData_LocalPos().c_str());
	m_pDREngine->DrawText(630, 230, _pPlayer->GetDebugData_WorldPos().c_str());
	m_pDREngine->DrawText(630, 250, _pPlayer->GetDebugData_Rotation().c_str());
	m_pDREngine->DrawText(630, 270, _pPlayer->GetDebugData_Scale().c_str());

	m_pDREngine->DrawText(630, 290, "----------------------------");

	CChildPlayer* _pChildPlayer = m_pMyChild->GetComponent2<CChildPlayer*>();

	m_pDREngine->DrawText(630, 310, _pChildPlayer->GetDebugData_Name().c_str());
	m_pDREngine->DrawText(630, 330, _pChildPlayer->GetDebugData_LocalPos().c_str());
	m_pDREngine->DrawText(630, 350, _pChildPlayer->GetDebugData_WorldPos().c_str());
	m_pDREngine->DrawText(630, 370, _pChildPlayer->GetDebugData_Rotation().c_str());
	m_pDREngine->DrawText(630, 410, _pChildPlayer->GetDebugData_Scale().c_str());


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
