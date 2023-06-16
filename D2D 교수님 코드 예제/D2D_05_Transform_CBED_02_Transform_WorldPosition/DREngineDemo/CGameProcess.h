#pragma once

// �ܺ� ���Ӽ����� �����Ѵ�.
#include "DRD2DEngine.h"	// Direct2D�� �̿��� �׷��Ƚ� ����
#include <vector>
#include "GameObject.h"

using namespace std;

/// ���� ���� �κ�
/// 
/// ���� ���࿡ ���� ��� ���� ����Ѵ�.
/// �����ӿ� (WinAPI32����, �׷��Ƚ� ����)
///
/// [12/18/2019 LeHide]

class CGameProcess
{
public:
	CGameProcess();
	~CGameProcess();

private:
	// �������� ����

	// ȭ���� ũ��
	const int m_ScreenWidth = 1024;
	const int m_ScreenHeight = 768;

	// ���� ����
	HWND m_hWnd;
	MSG m_Msg;

	// �׷��Ƚ� ����
	IDREngine* m_pDREngine;

	// �浹üũ �׽�Ʈ
	CBEngine::GameObject* m_pTargetObject;
	CBEngine::GameObject* m_pMyObject;
	CBEngine::GameObject* m_pMyChild;

	/// ������Ʈ ����� ���� ������
	std::vector<CBEngine::GameObject*> m_GameObjects;

private:
	// for Loop
	void UpdateAll();
	void RenderAll();

	bool m_KeyUp;
	bool m_KeyDown;
	bool m_KeyLeft;
	bool m_KeyRight;

public:
	// ���ӿ� ���� �����ӿ��� �ʱ�ȭ�Ѵ�.
	HRESULT Initialize(HINSTANCE hInstance);

	// ���� ���� ����. ���� ������ ����Ǵ� �ݺ�����
	void Loop();

	// ������ �����Ѵ�.
	void Finalize();


	// �޽��� �ڵ鷯 (���� �ݹ�)
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

