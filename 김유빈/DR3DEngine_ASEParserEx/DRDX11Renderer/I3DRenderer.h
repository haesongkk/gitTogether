#pragma once

///
/// 렌더러의 인터페이스
///
/// Software, DX9/DX11/DX12, OpenGL등의 서로 다른 환경에서 돌아 갈 수 있도록
///
/// [2021.02.06 LeHideHome]

class I3DRenderer abstract
{
public:
	I3DRenderer() {}
	virtual ~I3DRenderer() {};

	// 엔진을 초기화한다.
	virtual bool Initialize(int hinst, int hWnd, int screenWidth, int screenHeight) abstract;


	// 업데이트
	virtual void Update(float deltaTime) abstract;

	// 랜더링 시작
	virtual void BeginRender() abstract;

	// 랜더링 끝 (백버퍼에서 화면으로 Blit, 페이지플립핑, Present등...)
	virtual void EndRender() abstract;

	// 엔진을 종료한다.
	virtual void Finalize() abstract;


public:
	// 랜더러 개발 중 테스트 함수
	virtual void Draw_Test() abstract;

	// 창 변환 관련
	virtual void OnResize() abstract;
	virtual bool IsValidDevice() abstract;
	virtual void SetClientSize(int width, int height) abstract;
	virtual void SetWinMinMax(bool isMinimized, bool isMaxmized) abstract;

	// 마우스 입력. 윈도 메시지 기반
	virtual void OnMouseDown(int x, int y) abstract;
	virtual void OnMouseUp(int x, int y) abstract;
	virtual void OnMouseMove(int btnState, int x, int y) abstract;
};
