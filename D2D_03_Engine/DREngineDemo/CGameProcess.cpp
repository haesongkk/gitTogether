#include "CGameProcess.h"


HRESULT CGameProcess::Initialize(HINSTANCE hInstance)
{
	/// Win32 ����
	// ���� Ŭ����
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

	// ���� Ŭ���� ���
	RegisterClass(&wndclass);

	// ���� ����
	m_hWnd = CreateWindow(
		szAppName,
		szAppName,
		WS_OVERLAPPEDWINDOW,
		100, 100, m_ScreenWidth, m_ScreenHeight,
		NULL, NULL, hInstance, NULL);

	if (!m_hWnd) return S_FALSE;

	// ������ ������ ȭ�鿡 ǥ��
	ShowWindow(m_hWnd, SW_SHOWNORMAL);
	UpdateWindow(m_hWnd);


	/// �׷��Ƚ� ���� ����

	// ��Ʈ�� �ܺ� ������
	m_SpritePool.clear();

	// �׷��Ƚ� ���� �ʱ�ȭ
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
	/// ȭ���� �����.
	m_pDREngine->ClearScreen();



	// ���� ���
	//m_pDREngine->DrawText(10, 10, "�ƾƾ�");
	//m_pDREngine->DrawText(630, 100, "Status");

	///m_pSLEngine->_TT(630, 150, "ElapsedTime %d", t.getElapsedTime());

	m_pDREngine->DrawText(630, 270, "----------------------------");
	m_pDREngine->DrawText(630, 410, "���� ��� Ȯ����");
	m_pDREngine->DrawText(630, 550, "�Ͼ� Ÿ��: ��");
	m_pDREngine->DrawText(630, 690, "���λ� �׸�: �ֹ���");
	m_pDREngine->DrawText(630, 840, "������ �׸�: �ϰ���");
	//m_pDREngine->DrawText(630, 270, "//");
	m_pDREngine->DrawText(630, 990, "���� Ÿ��: ���հ���");

	//m_pDREngine->DrawSprite(0, 100, 100);



	/// ������� ������ ȭ������ ����Ѵ�.
	m_pDREngine->Render();
}

void CGameProcess::Finalize()
{
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
