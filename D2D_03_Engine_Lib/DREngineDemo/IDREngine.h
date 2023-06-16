#pragma once
#include <windows.h>
#include "DREngineDefine.h"

/// Darong 2D Engine Interface
///
/// 상속해서 WinAPI, Direct2D등을 사용 가능하다.
/// 모두 추상화가 되어 있으며, windows.h정도를 사용한다.
///
/// [12/27/2019 LeHide]
///

class IDREngine abstract
{
protected:
	static IDREngine* m_Instance;

public:
	//IDREngine();
	//~IDREngine();

	static IDREngine* GetInstance() { return m_Instance; }

	///--------------------------------------------------------------------------------
	/// 초기화, 그리기, 끝내기

	// 초기화 관련
	virtual void Initialize(HINSTANCE hinst, HWND hWnd, int screenWidth, int screenHeight) abstract;
	virtual void ChangeDisplay(int screenWidth, int screenHeight) abstract;
	virtual void RestoreDisplay() abstract;
	virtual void ClearScreen() abstract;

	// 화면에 최종적으로 그린다.
	virtual void Render() abstract;

	// 엔진을 종료한다.
	virtual void Finalize() abstract;


	///--------------------------------------------------------------------------------
	/// 그리기 관련 Primitive

	// 지정된 좌표에 사각형을 출력한다. 이런식으로 장치 독립적으로 가야 한다.
	virtual void DrawRectangle(int x, int y, int x1, int y1, COLORREF crBrush) abstract;
	virtual void DrawRectangle(int x, int y, int x1, int y1) abstract;
	virtual void DrawLine(int, int, int, int) abstract;
	virtual void DrawEllipse(int x, int y) abstract;
	virtual void DrawEllipse(int left, int top, int right, int bottom) abstract;
	virtual void DrawSelectRectangle(int x1, int y1, int x2, int y2) abstract;
	virtual void DrawSelectedCircle(int x, int y, int size) abstract;

	// 화면에 문자를 출력
	virtual bool DrawText(int x, int y, const char* pch, ...) abstract;


	/// ----------------------------------------
	/// 그리기 관련 Sprite

	// 시트에서 로드하는 버전
	virtual int LoadSpriteSheet(char* filePath) abstract;
	virtual DRSprite* LoadSpriteFromSheet(char* filePath) abstract;
	virtual DRSprite* LoadSpriteFromSheet(int sheetIndex, int oriX, int oriY, int width, int height, int delayFrame) abstract;


	// 바로 이미지 파일을 하나 읽는 버전
	///virtual DRSprite* LoadSprite(char* filePath) abstract;
	///virtual DRSprite* GetSprite(int index) abstract;

	
	// 스프라이트 출력
	// 원본 위치 등의 정보는 엔진별 구현이며 숨겨져 있다.
	virtual void DrawSprite(int spriteIndex, int dstX, int dstY) abstract;
	virtual void DrawSprite(DRSprite* sprite, int dstX, int dstY) abstract;


	// 모든 스프라이트/스프라이트 시트 제거
	virtual void RemoveAllSprites() abstract;
	virtual void RemoveAllSpriteSheets() abstract;
};

/// 상속한 CPP파일로
/// IDREngine* IDREngine::m_Instance = nullptr;


/// virtual DRSprite* LoadSprite(char* filePath) abstract;
// 엔진마다 다르게 동작 할 수 있다. Direct2D같은 경우, 바로 하나의 이미지 파일을 읽을 수도 있고,
// WinAPI같은 경우 스프라이트 시트의 인덱스와, 상대적인 위치만 알아도 된다.
// 그 경우, 그 정보를 담은 파일을 로드하는 것일수도 있고.. 등등

