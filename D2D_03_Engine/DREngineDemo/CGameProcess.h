#pragma once

// �ܺ� ���Ӽ����� �����Ѵ�.
///#include "SLEngine.h"	// WinAPI(GDI)�� �̿��� �׷��Ƚ� ����
#include "DRD2DEngine.h"	// Direct2D�� �̿��� �׷��Ƚ� ����
#include <vector>

// ȭ���� ũ��
#define SCREENWIDTH 1920
#define SCREENHEIGHT 1080

using namespace std;

/// ���� ���� �κ�
/// 
/// ���� ���࿡ ���� ��� ���� ����Ѵ�.
/// �����ӿ� (WinAPI32����, �׷��Ƚ� ����)
///
/// [12/18/2019 LeHide]

class CGameProcess
{
private:
	// �������� ����
	const int m_ScreenWidth = 1920;
	const int m_ScreenHeight = 1080;


	// ���� ����
	HWND m_hWnd;
	MSG m_Msg;


	// �׷��Ƚ� ����
	IDREngine* m_pDREngine;

	/// ��������Ʈ Ǯ. �׳� �޾ƺô�. ��� �̷��� �����Ű�� ���⵵�� �����Ѵ�.
	vector<DRSprite*> m_SpritePool;

public:
	// ���ӿ� ���� �����ӿ��� �ʱ�ȭ�Ѵ�.
	HRESULT Initialize(HINSTANCE hInstance);


	// ���� ���� ����. ���� ������ ����Ǵ� �ݺ�����
	void Loop();
	void GameLoop();


	// ������ �����Ѵ�.
	void Finalize();


	// �޽��� �ڵ鷯 (���� �ݹ�)
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

