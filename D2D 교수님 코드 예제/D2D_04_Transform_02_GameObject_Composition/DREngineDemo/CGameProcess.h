#pragma once

// 외부 종속성으로 동작한다.
#include "DRD2DEngine.h"	// Direct2D를 이용한 그래픽스 엔진
#include <vector>
#include "GameObject.h"

using namespace std;

class Traditional::GameObject;
class CPlayer;
class CChildPlayer;

/// 게임 진행 부분
/// 
/// 게임 진행에 관한 모든 것을 담당한다.
/// 프레임웍 (WinAPI32관련, 그래픽스 엔진)
///
/// [12/18/2019 LeHide]

class CGameProcess
{
public:
	CGameProcess();
	~CGameProcess();

private:
	// 공통적인 설정

	// 화면의 크기
	const int m_ScreenWidth = 1024;
	const int m_ScreenHeight = 768;

	// 윈도 관련
	HWND m_hWnd;
	MSG m_Msg;

	// 그래픽스 엔진
	IDREngine* m_pDREngine;

	// 변환을 이용한 움직임 테스트
	Traditional::GameObject* m_pTargetObject;
	CPlayer* m_pMyObject;
	CChildPlayer* m_pMyChild;

	/// 게임오브젝트들을 관리
	std::vector<Traditional::GameObject*> m_GameObjects;

private:
	// for Loop
	void UpdateAll();
	void RenderAll();


public:
	// 게임용 윈도 프레임웍을 초기화한다.
	HRESULT Initialize(HINSTANCE hInstance);

	// 메인 게임 루프. 실제 게임이 진행되는 반복구문
	void Loop();

	// 게임을 정리한다.
	void Finalize();


	// 메시지 핸들러 (윈도 콜백)
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

