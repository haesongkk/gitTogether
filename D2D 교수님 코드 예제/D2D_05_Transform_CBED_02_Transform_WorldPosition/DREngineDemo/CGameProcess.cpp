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
	/// Win32 관련
	// 윈도 클래스
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

	/// 컴포넌트 기반의 엔진 디자인

	
	/// 게임오브젝트 1

	// 엔티티 생성
	m_pTargetObject = new CBEngine::GameObject("TargetObject");

	// 변환 컴포넌트
	Transform* _newTransform = new Transform();
	_newTransform->SetLocalPosition(CVECTOR2(300, 310));
	_newTransform->SetLocalRotation(15.f);
	_newTransform->SetLocalScale(CVECTOR2(3.0f, 1.0f));

	/// TM 역변환 테스트
	//_newTransform->MakeTransformMatrix();
	//CVECTOR2 _position = _newTransform->GetPositionFromTM(_newTransform->GetTransformMatrix());
	//float _angle = _newTransform->GetRotationFromTM(_newTransform->GetTransformMatrix());
	//CVECTOR2 _scale = _newTransform->GetScaleFromTM(_newTransform->GetTransformMatrix());


	
	// 랜더러 컴포넌트
	PrimitiveRenderer2D* _newRenderer = new PrimitiveRenderer2D();
	_newRenderer->primitiveType = eCollisionType::eCC;
	_newRenderer->m_Radius = 100.0f;

	// 컴포넌트를 게임오브젝트(엔티티)에 Add
	m_pTargetObject->AddComponent<Transform*>(_newTransform);
	m_pTargetObject->AddComponent<PrimitiveRenderer2D*>(_newRenderer);



	/// 게임오브젝트 2
	m_pMyObject = new CBEngine::GameObject("MyObject");

	Transform* _newTransform2 = new Transform();
	_newTransform2->SetLocalPosition(CVECTOR2(400, 400));
	_newTransform2->SetLocalRotation(0);
	_newTransform2->SetLocalScale(CVECTOR2(1.f, 1.f));

	// 랜더러 컴포넌트
	PrimitiveRenderer2D* _newRenderer2 = new PrimitiveRenderer2D();
	_newRenderer2->primitiveType = eCollisionType::eCC;
	_newRenderer2->m_Radius = 100.0f;

	// 유저가 만든 핸들링용 컴포넌트
	CPlayer* _myPlayer = new CPlayer();

	// 컴포넌트를 게임오브젝트(엔티티)에 Add
	m_pMyObject->AddComponent<Transform*>(_newTransform2);
	m_pMyObject->AddComponent<PrimitiveRenderer2D*>(_newRenderer2);
	m_pMyObject->AddComponent(_myPlayer);


	/// 게임오브젝트 3 (2의 Child)
	m_pMyChild = new CBEngine::GameObject("MyChildObject");

	Transform* _newTransform3 = new Transform();
	_newTransform3->SetLocalPosition(CVECTOR2(50, 50));	// 로컬좌표계이다
	_newTransform3->SetLocalRotation(-45.f);				// 마찬가지
	_newTransform3->SetLocalScale(CVECTOR2(1.f, 1.f));

	// 오브젝트2에 부모-자식 관계로 연결해준다.
	//_newTransform3->SetParent(_newTransform2);

	// 랜더러 컴포넌트
	PrimitiveRenderer2D* _newRenderer3 = new PrimitiveRenderer2D();
	_newRenderer3->primitiveType = eCollisionType::eCC;
	_newRenderer3->m_Radius = 30.0f;


	// 유저가 만든 핸들링용 컴포넌트
	CChildPlayer* _childPlayer = new CChildPlayer();

	// 컴포넌트를 게임오브젝트(엔티티)에 Add
	m_pMyChild->AddComponent<Transform*>(_newTransform3);
	m_pMyChild->AddComponent<PrimitiveRenderer2D*>(_newRenderer3);
	m_pMyChild->AddComponent<CChildPlayer*>(_childPlayer);

	//
	//m_pMyChild->GetComponent<Transform>()->SetParent(m_pMyObject->GetComponent<Transform>());
	m_pMyChild->GetComponent2<Transform*>()->SetParent(m_pMyObject->GetComponent<Transform>());

	// 컴포넌트에서 컴포넌트를 호출
	Transform* _componentGetFromComponent1 = _childPlayer->GetComponent<Transform>();
	Transform* _componentGetFromComponent2 = _childPlayer->GetComponent2<Transform*>();



	/// GetComponent 테스트
	//Transform* _componentFinded = m_pTargetObject->GetComponent<Transform>();	// 템플릿 안쪽이 포인터가 아님	
	//Transform* _componentFinded2 = m_pTargetObject->GetComponent2<Transform*>();// 같도록 맞춰줬음
	//Renderer* _renderer = m_pTargetObject->GetComponent2<Renderer*>();

	/// 게임 오브젝트 리스트에 넣고 관리함
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
	// 키 입력
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


	// 오브젝트 이동
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

	m_pDREngine->DrawText(10, 10, "한글일곱자출력A1c");
	m_pDREngine->DrawText(10, 30, "MyObject : 화살표 - 이동 / SpaceBar - 회전 / Enter - 정지 / P - WPos(10,10)강제이동");
	m_pDREngine->DrawText(10, 50, "MyChild : WSAD - 이동 / XC - 회전 / Z - 정지");


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
