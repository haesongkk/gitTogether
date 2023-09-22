#pragma once

///
/// 랜더러의 인터페이스
///
/// Software, DX12, DX9, OpenGL등
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

	virtual void OnMouseDown(int x, int y) abstract;
	virtual void OnMouseUp(int x, int y) abstract;
	virtual void OnMouseMove(int btnState, int x, int y) abstract;
};