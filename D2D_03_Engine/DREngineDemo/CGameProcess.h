#pragma once

// 외부 종속성으로 동작한다.
///#include "SLEngine.h"	// WinAPI(GDI)를 이용한 그래픽스 엔진
#include "DRD2DEngine.h"	// Direct2D를 이용한 그래픽스 엔진
#include <vector>

// 화면의 크기
#define SCREENWIDTH 1920
#define SCREENHEIGHT 1080

using namespace std;

/// 게임 진행 부분
/// 
/// 게임 진행에 관한 모든 것을 담당한다.
/// 프레임웍 (WinAPI32관련, 그래픽스 엔진)
///
/// [12/18/2019 LeHide]

class CGameProcess
{
private:
	// 공통적인 설정
	const int m_ScreenWidth = 1920;
	const int m_ScreenHeight = 1080;


	// 윈도 관련
	HWND m_hWnd;
	MSG m_Msg;


	// 그래픽스 엔진
	IDREngine* m_pDREngine;

	/// 스프라이트 풀. 그냥 받아봤다. 사실 이렇게 노출시키면 복잡도가 증가한다.
	vector<DRSprite*> m_SpritePool;

public:
	// 게임용 윈도 프레임웍을 초기화한다.
	HRESULT Initialize(HINSTANCE hInstance);


	// 메인 게임 루프. 실제 게임이 진행되는 반복구문
	void Loop();
	void GameLoop();


	// 게임을 정리한다.
	void Finalize();


	// 메시지 핸들러 (윈도 콜백)
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

