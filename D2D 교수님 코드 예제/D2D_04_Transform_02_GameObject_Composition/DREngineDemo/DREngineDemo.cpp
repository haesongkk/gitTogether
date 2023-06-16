﻿//
// DREngineDemo
//
// Direct2D 엔진 데모
// 부활한 게임아카데미의 교육용 자료로 생성함
//
// Game Academy 2021
//
// [01/31/2021 LeHideHome]
//

// 2D Game Programming 4강 04_Transform 예제
// 
#include <windows.h>
#include "CGameProcess.h"

// 프로그램의 엔트리 포인트. 멀티바이트 프로젝트 기준
int WINAPI WinMain(_In_ HINSTANCE hInstance, 
	_In_opt_ HINSTANCE hPrevInstance, 
	_In_ LPTSTR lpCmdLine, _In_ int nCmdShow)
{
	// 게임 진행 클래스
	// 내부에서 윈도 생성과 메시지 콜백을 처리한다.
	// 또한 그래픽스 엔진 등을 포함한다.
	CGameProcess* pGameProcess = new CGameProcess();
	pGameProcess->Initialize(hInstance);
	pGameProcess->Loop();
	pGameProcess->Finalize();

	return 0; // (int)msg.wParam;
}
