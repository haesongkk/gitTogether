#include "CGameProcess.h"


HRESULT CGameProcess::Initialize(HINSTANCE hInstance)
{
	/// Win32 관련
	// 윈도 클래스
	char szAppName[] = "Game Institute of Technology 2022";
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

	if (!m_hWnd) return S_FALSE;

	// 생성된 윈도를 화면에 표시
	ShowWindow(m_hWnd, SW_SHOWNORMAL);
	UpdateWindow(m_hWnd);


	/// 그래픽스 엔진 관련

	// 비트맵 외부 보관용
	m_SpritePool.clear();

	// 그래픽스 엔진 초기화
	//m_pDREngine = new SLEngine();
	m_pDREngine = new DRD2DEngine();

	m_pDREngine->Initialize(hInstance, m_hWnd, m_ScreenWidth, m_ScreenHeight);

	m_pDREngine->LoadSpriteSheet((char*)"./Images/atk_1.bmp");
	m_SpritePool.push_back(m_pDREngine->LoadSpriteFromSheet(0, 0, 0, 100, 140, 1));

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
			GameLoop();
		}
	}
}

void CGameProcess::GameLoop()
{
	/// 화면을 지운다.
	m_pDREngine->ClearScreen();



	// 상태 출력
	//m_pDREngine->DrawText(10, 10, "아아아");
	//m_pDREngine->DrawText(630, 100, "Status");

	///m_pSLEngine->_TT(630, 150, "ElapsedTime %d", t.getElapsedTime());

	m_pDREngine->DrawText(630, 270, "----------------------------");
	m_pDREngine->DrawText(630, 410, "문장 출력 확인중");
	m_pDREngine->DrawText(630, 550, "하얀 타원: 알");
	m_pDREngine->DrawText(630, 690, "연두색 네모: 애벌레");
	m_pDREngine->DrawText(630, 840, "빨간색 네모: 일개미");
	//m_pDREngine->DrawText(630, 270, "//");
	m_pDREngine->DrawText(630, 990, "검정 타원: 여왕개미");

	//m_pDREngine->DrawSprite(0, 100, 100);



	/// 백버퍼의 내용을 화면으로 출력한다.
	m_pDREngine->Render();
}

void CGameProcess::Finalize()
{
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
