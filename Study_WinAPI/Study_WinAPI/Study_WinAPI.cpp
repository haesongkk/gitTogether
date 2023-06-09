﻿// Study_WinAPI.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "Study_WinAPI.h"
#include "InputManager.h"

#define MAX_LOADSTRING 100

void UpdatePlayer();

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.
HWND hMainWnd;

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
    LoadStringW(hInstance, IDC_STUDYWINAPI, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_STUDYWINAPI));

    MSG msg;

    InputManager::GetInst()->InitInput();
    UpdatePlayer();

    //while (true)
    {
        //InputManager::GetInst()->UpdateInput();
        
        //if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        //{
        //   
        //    {
              //DispatchMessage(&msg);
        //    }
        //}
    }

    //return (int) msg.wParam;
}
static int x = 10;
static int y = 10;

// 플레이어 이동
void UpdatePlayer()
{

    while (InputManager::GetInst()->GetKeyState(KEY::LEFT) != KEY_STATE::AWAY)
    {
        InputManager::GetInst()->UpdateInput();

        //if (InputManager::GetInst()->GetKeyState(KEY::LEFT) == KEY_STATE::TAP)
        //{

            // Device Context 만들어서 ID 를 반환
            //HDC hdc = BeginPaint(hWnd, &ps);    // Device Context (그리기 위한)
            // DC 의 목적지는 hWnd
            // DC 의 펜은 기본펜(Black)
            // DC 의 브러쉬는 기본 브러쉬(White)



            // 윈도우의 핸들
            // 윈도우의 좌표
            // HDC
            //Rectangle(hdc, x, y, 110, 110);

            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
            //EndPaint(hWnd, &ps);
            //x++;
        //}
    }
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_STUDYWINAPI));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_STUDYWINAPI);
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
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   // OS 에서 윈도우 창에 대한 아이디를 받음 -> 핸들로 다룰 수 있다
   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

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

    // ** 스위치문 사용시에 case 문 내부 변수를 선언할 경우 중괄호가 꼭 필요함
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



    // 무효화 영역이란
    // 다시 그림을 채워야하는 경우
    // ex. 창 최소화 후 다시 킬 경우 -> 화면 전체에 대해 무효화 영역 발생

    case WM_PAINT:  // 무효화 영역 (Invalidate)이 발생한 경우
        {
            PAINTSTRUCT ps;

            struct __HWND
            {
                int a;
            }typedef HWND;

            HWND;
            HPEN;
            HBRUSH;

            // Device Context 만들어서 ID 를 반환
            HDC hdc = BeginPaint(hWnd, &ps);    // Device Context (그리기 위한)
            // DC 의 목적지는 hWnd
            // DC 의 펜은 기본펜(Black)
            // DC 의 브러쉬는 기본 브러쉬(White)



            // 윈도우의 핸들
            // 윈도우의 좌표
            // HDC
            //Rectangle(hdc, x, y, 110, 110);

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
