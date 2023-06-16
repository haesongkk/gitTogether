#pragma once

// �ܺ� ���Ӽ����� �����Ѵ�.
#include "DRD2DEngine.h"	// Direct2D�� �̿��� �׷��Ƚ� ����
#include <vector>
#include "GameObject.h"

using namespace std;

class Traditional::GameObject;
class CPlayer;
class CChildPlayer;

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

	// ��ȯ�� �̿��� ������ �׽�Ʈ
	Traditional::GameObject* m_pTargetObject;
	CPlayer* m_pMyObject;
	CChildPlayer* m_pMyChild;

	/// ���ӿ�����Ʈ���� ����
	std::vector<Traditional::GameObject*> m_GameObjects;

private:
	// for Loop
	void UpdateAll();
	void RenderAll();


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

