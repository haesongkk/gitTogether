// D2DPractice.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include <windows.h>
#include "framework.h"
#include "D2DPractice.h"
#include "D2DEngineLib.h"
#pragma comment(lib, "D2DEngineLib.lib")
//#pragma comment(lib, "d2d1" )
#pragma comment(lib, "windowscodecs")

#include "Circle.h"
#include "MyRectangle.h"
#include "InputManager.h"
#include "CC.h"
#include "AABB.h"
#include "Vector2.h"
#include "BoundingBox.h"
#include "OBB.h"

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 화면 사이즈
const int screenWidth = 1500;
const int screenHeight = 1000;

// 윈도우 관련
HWND hWnd;

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_D2DPRACTICE, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    //// 애플리케이션 초기화를 수행합니다:
    //if (!InitInstance (hInstance, nCmdShow))
    //{
    //    return FALSE;
    //}

	hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

	hWnd = CreateWindow(
        szWindowClass,
        szTitle,
		WS_OVERLAPPEDWINDOW,
		100, 100, screenWidth, screenHeight,
		NULL, NULL, hInstance, NULL);

	if (!hWnd) return S_FALSE;

	// 생성된 윈도를 화면에 표시
	ShowWindow(hWnd, SW_SHOWNORMAL);
	UpdateWindow(hWnd);

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_D2DPRACTICE));

    MSG msg;

    InputManager::GetInstance()->Initialize();
    InputManager::GetInstance()->SetMainHwnd(hWnd);

	D2DEngineLib* pD2DEngine = new D2DEngineLib;
    pD2DEngine->Initialize(hInstance, hWnd, screenWidth, screenHeight);

    /// 초기화

    DWORD defaultColor = RGB(0, 0, 0);
    DWORD activeColor = RGB(255, 0, 0);
    DWORD collisionColor = RGB(0, 0, 255);

    /// Set circle
    Circle* m_pCircle1 = new Circle;
    m_pCircle1->m_posx = 250;
    m_pCircle1->m_posy = 150;
    m_pCircle1->m_radius = 50;

	Circle* m_pCircle2 = new Circle;
	m_pCircle2->m_posx = 400;
	m_pCircle2->m_posy = 450;
	m_pCircle2->m_radius = 70;

	Circle* m_pCircle3 = new Circle;
	m_pCircle3->m_posx = 200;
	m_pCircle3->m_posy = 480;
	m_pCircle3->m_radius = 70;

    DWORD circleColor1 = defaultColor;
    DWORD circleColor2 = defaultColor;
    DWORD circleColor3 = defaultColor;

    /// Set rectangle
    MyRectangle* m_pRectangle1 = new MyRectangle;
	m_pRectangle1->SetRectangle(700, 900, 250, 400);

    MyRectangle* m_pRectangle2 = new MyRectangle;
    m_pRectangle2->SetRectangle(750, 1000, 500, 600);

    MyRectangle* m_pRectangle3 = new MyRectangle;
    m_pRectangle3->SetRectangle(1050, 1250, 400, 700);
	
    DWORD rectColor1 = defaultColor;
    DWORD rectColor2 = defaultColor;
    DWORD rectColor3 = defaultColor;

    /// Set OBB Box
    BoundingBox* m_pBoundingBox1 = new BoundingBox;
    BoundingBox* m_pBoundingBox2 = new BoundingBox;
    BoundingBox* m_pBoundingBox3 = new BoundingBox;
    
    m_pBoundingBox1->SetRectangle(Vector2( 600.f, 100.f ), Vector2( 800.f, 100.f ),
        Vector2( 600.f, 200.f ), Vector2( 800.f, 200.f ));
    m_pBoundingBox2->SetRectangle(Vector2(200.f, 100.f), Vector2(500.f, 100.f),
        Vector2(200.f, 300.f), Vector2(500.f, 300.f));
    m_pBoundingBox3->SetRectangle(Vector2(350.f, 400.f), Vector2(700.f, 400.f),
        Vector2(350.f, 600.f), Vector2(700.f, 600.f));
    
    DWORD boxColor1 = defaultColor;
    DWORD boxColor2 = defaultColor;
    DWORD boxColor3 = defaultColor;

    /// Set collision detector
    CC* m_CC1 = new CC;
    CC* m_CC2 = new CC;
    m_CC1->SetTarget(m_pCircle1, m_pCircle2);
    m_CC2->SetTarget(m_pCircle1, m_pCircle3);

    AABB* m_AABB1 = new AABB;
    AABB* m_AABB2 = new AABB;
    m_AABB1->SetTarget(m_pRectangle1, m_pRectangle2);
    m_AABB2->SetTarget(m_pRectangle1, m_pRectangle3);

    OBB* m_OBB1 = new OBB;
    OBB* m_OBB2 = new OBB;
    m_OBB1->SetTarget(m_pBoundingBox1, m_pBoundingBox2);
    m_OBB2->SetTarget(m_pBoundingBox1, m_pBoundingBox3);

    bool activateCC = false;
    bool activateAABB = false;
    bool activateOBB = false;
            
	// 기본 메시지 루프입니다:
	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
            InputManager::GetInstance()->Update();
            
            /// CC 
			circleColor2 = defaultColor;
			circleColor3 = defaultColor;

            if (m_pCircle1->isClicked)
            {
                activateCC = true;


                circleColor1= activeColor;
                circleColor2 = activeColor;
                circleColor3 = activeColor;

                /// CC Collision Check
                if (m_CC1->CollisionCheck())
                {
                    circleColor1 = collisionColor;
                    circleColor2 = collisionColor;
                }
                if (m_CC2->CollisionCheck())
                {
                    circleColor1 = collisionColor;
                    circleColor3 = collisionColor;
                }
            }
            else
            {
                activateCC = false;
                circleColor1 = defaultColor;
            }

            /// AABB
            rectColor2 = defaultColor;
            rectColor3 = defaultColor;

            if (m_pRectangle1->isClicked )
            {
                activateAABB = true;
                

                rectColor1 = activeColor;
                rectColor2 = activeColor;
                rectColor3 = activeColor;

                if (m_AABB1->CollisionCheck())
                {
                    rectColor1 = collisionColor;
                    rectColor2 = collisionColor;
                }
                if (m_AABB2->CollisionCheck())
                {
                    rectColor1 = collisionColor;
                    rectColor3 = collisionColor;
                }
            }
            else
            {
                activateAABB = false;
                rectColor1 = defaultColor;
            }

            boxColor2 = defaultColor;
            boxColor3 = defaultColor;

            /// OBB
            if (m_pBoundingBox1->isClicked)
            {
                activateOBB = true;
                
                boxColor1 = activeColor;
                boxColor2 = activeColor;
                boxColor3 = activeColor;

                if (m_OBB1->CollisionCheck())
                {
                    boxColor1 = collisionColor;
                    boxColor2 = collisionColor;
                }
                if (m_OBB2->CollisionCheck())
                {
                    boxColor1 = collisionColor;
                    boxColor3 = collisionColor;
                }
            }
            else
            {
                activateOBB = false;
                boxColor1 = defaultColor;
            }
            m_pCircle1->Update();
            m_pRectangle1->Update();
            m_pBoundingBox1->Update();

            /// Begin Render
            pD2DEngine->BeginDraw();
            pD2DEngine->ClearScreen();
            // 임의의 네점으로 사각형 그리기 함수 테스트
            pD2DEngine->DrawSelectRectangle({ 20, 650 }, { 480, 650 }, { 20, 800 }, { 480, 800 }, RGB(0, 0, 0));
            pD2DEngine->DrawLine({ 20, 700 }, { 480, 700 }, RGB(0, 0, 0));
            pD2DEngine->DrawLine({ 20, 750 }, { 480, 750 }, RGB(0, 0, 0));
            // txt test
            pD2DEngine->DrawText(900, 65, "[Collision Detection Assignment]\n\n좌하단 클릭->조작:방향키/Q/E ", 0, RGB(0, 0, 0));
            pD2DEngine->DrawText(10, 650, "Click To Activate CC", 1, circleColor1);
            pD2DEngine->DrawText(10, 700, "Click To Activate AABB", 1, rectColor1);
            pD2DEngine->DrawText(10, 750, "Click To Activate OBB", 1, boxColor1);
            // mouse cursor
            pD2DEngine->DrawSelectedCircle(InputManager::GetInstance()->m_MousePos.x, InputManager::GetInstance()->m_MousePos.y, 15, collisionColor);

            /// OBB Box Render
            if(activateOBB)
            {
                pD2DEngine->DrawSelectRectangle
                ({ (LONG)m_pBoundingBox1->m_leftTop.x, (LONG)m_pBoundingBox1->m_leftTop.y },
                 { (LONG)m_pBoundingBox1->m_rightTop.x, (LONG)m_pBoundingBox1->m_rightTop.y },
                 { (LONG)m_pBoundingBox1->m_leftBottom.x,(LONG)m_pBoundingBox1->m_leftBottom.y },
                 { (LONG)m_pBoundingBox1->m_rightBottom.x, (LONG)m_pBoundingBox1->m_rightBottom.y },
                    boxColor1);
                pD2DEngine->DrawSelectRectangle
                (   { (LONG)m_pBoundingBox2->m_leftTop.x, (LONG)m_pBoundingBox2->m_leftTop.y },
                    { (LONG)m_pBoundingBox2->m_rightTop.x, (LONG)m_pBoundingBox2->m_rightTop.y },
                    { (LONG)m_pBoundingBox2->m_leftBottom.x,(LONG)m_pBoundingBox2->m_leftBottom.y },
                    { (LONG)m_pBoundingBox2->m_rightBottom.x, (LONG)m_pBoundingBox2->m_rightBottom.y },
                    boxColor2);
                pD2DEngine->DrawSelectRectangle
                (   { (LONG)m_pBoundingBox3->m_leftTop.x, (LONG)m_pBoundingBox3->m_leftTop.y },
                    { (LONG)m_pBoundingBox3->m_rightTop.x, (LONG)m_pBoundingBox3->m_rightTop.y },
                    { (LONG)m_pBoundingBox3->m_leftBottom.x,(LONG)m_pBoundingBox3->m_leftBottom.y },
                    { (LONG)m_pBoundingBox3->m_rightBottom.x, (LONG)m_pBoundingBox3->m_rightBottom.y },
                    boxColor3);
            }


            /// Rectangle Render
            if (activateAABB)
            {
                pD2DEngine->DrawRectangle(m_pRectangle1->m_Left, m_pRectangle1->m_Top, m_pRectangle1->m_Right, m_pRectangle1->m_Bottom, rectColor1);
                pD2DEngine->DrawRectangle(m_pRectangle2->m_Left, m_pRectangle2->m_Top, m_pRectangle2->m_Right, m_pRectangle2->m_Bottom, rectColor2);
                pD2DEngine->DrawRectangle(m_pRectangle3->m_Left, m_pRectangle3->m_Top, m_pRectangle3->m_Right, m_pRectangle3->m_Bottom, rectColor3);
            }


            /// Circle Render
            if (activateCC)
            {
                pD2DEngine->DrawSelectedCircle(m_pCircle1->m_posx, m_pCircle1->m_posy, m_pCircle1->m_radius, circleColor1);
                pD2DEngine->DrawSelectedCircle(m_pCircle2->m_posx, m_pCircle2->m_posy, m_pCircle2->m_radius, circleColor2);
                pD2DEngine->DrawSelectedCircle(m_pCircle3->m_posx, m_pCircle3->m_posy, m_pCircle3->m_radius, circleColor3);
            }

            /// End Render
            pD2DEngine->EndDraw();
		}
	}

    return (int) msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_D2DPRACTICE));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_D2DPRACTICE);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
//BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
//{
//   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.
//
//   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
//      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);
//
//   if (!hWnd)
//   {
//      return FALSE;
//   }
//
//   ShowWindow(hWnd, nCmdShow);
//   UpdateWindow(hWnd);
//
//   return TRUE;
//}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
