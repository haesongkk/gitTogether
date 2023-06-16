#pragma once

/// Direct2d 그래픽스 엔진
///
/// 인터페이스를 통해서, WinAPI 그래픽스 엔진과 교체 가능하다.
///
/// [11/16/2019 LeHideHome]

#include "IDREngine.h"
#include "DRMacro.h"
#include <d2d1.h>
#include <d2d1_1.h>
#include "d2d1helper.h"		// ColorF
#include <dwrite.h>			// writefactory
#include <wincodec.h>		// WIC


using namespace D2D1;

class DRD2DEngine : public IDREngine
{
public:
	DRD2DEngine();
	virtual ~DRD2DEngine();

	/// IDREngine override
public:
	///--------------------------------------------------------------------------------
	/// 초기화 관련

	// 초기화 관련
	virtual void Initialize(HINSTANCE hinst, HWND hWnd, int screenWidth, int screenHeight) override;
	virtual void ChangeDisplay(int screenWidth, int screenHeight) override;
	virtual void RestoreDisplay() override;
	virtual void ClearScreen() override;

	// 화면에 그리기를 한다.
	virtual void Render() override;

	void DiscardDeviceResources();

	// 엔진을 종료한다.
	virtual void Finalize() override;

	///--------------------------------------------------------------------------------
	/// 그리기 관련

	// 지정된 좌표에 사각형을 출력한다. 이런식으로 장치 독립적으로 가야 한다.
	virtual void DrawRectangle(int x, int y, int x1, int y1, COLORREF crBrush) override;
	virtual void DrawRectangle(int x, int y, int x1, int y1) override;
	virtual void DrawLine(int, int, int, int) override;
	virtual void DrawEllipse(int x, int y) override;
	virtual void DrawEllipse(int left, int top, int right, int bottom) override;
	virtual void DrawSelectRectangle(int x1, int y1, int x2, int y2) override;
	virtual void DrawSelectedCircle(int x, int y, int size) override;

	// 화면에 문자를 출력
	virtual bool DrawText(int x, int y, const char* pch, ...) override;


	/// ----------------------------------------
	/// 그리기 관련 Sprite

	// 시트에서 로드하는 버전
	virtual int LoadSpriteSheet(char* filePath) override;
	virtual DRSprite* LoadSpriteFromSheet(char* filePath) override;
	virtual DRSprite* LoadSpriteFromSheet(int sheetIndex, int oriX, int oriY, int width, int height, int delayFrame) override;


	// 바로 이미지 파일을 하나 읽는 버전
	///virtual DRSprite* LoadSprite(char* filePath) abstract;
	///virtual DRSprite* GetSprite(int index) abstract;


	// 스프라이트 출력
	// 원본 위치 등의 정보는 엔진별 구현이며 숨겨져 있다.
	virtual void DrawSprite(int spriteIndex, int dstX, int dstY) override;
	virtual void DrawSprite(DRSprite* sprite, int dstX, int dstY) override;


	// 모든 스프라이트/스프라이트 시트 제거
	virtual void RemoveAllSprites() override;
	virtual void RemoveAllSpriteSheets() override;

private:
	HRESULT CreateDeviceResources();



	HRESULT LoadResourceBitmap(
		ID2D1RenderTarget* pRenderTarget,
		IWICImagingFactory* pIWICFactory,
		PCWSTR resourceName,
		PCWSTR resourceType,
		UINT destinationWidth,
		UINT destinationHeight,
		ID2D1Bitmap** ppBitmap
	);

	HRESULT LoadBitmapFromFile(
		ID2D1RenderTarget* pRenderTarget,
		IWICImagingFactory* pIWICFactory,
		PCWSTR uri,
		UINT destinationWidth,
		UINT destinationHeight,
		ID2D1Bitmap** ppBitmap
	);



private:
	HWND m_hWnd;								// 그리기를 할 윈도 (1개라고 가정)

	// Device Independant Resources
	ID2D1Factory* m_pD2DFactory;				// D2D는 Factory를 생성하면서 시작한다.

	IWICImagingFactory* m_pWICFactory;

	IDWriteFactory* m_pDWriteFactory;
	IDWriteTextFormat* m_pTextFormat;
	ID2D1SolidColorBrush* m_pWhiteBrush;


	// Device Dependant Resources
	ID2D1HwndRenderTarget* m_pRenderTarget;		// 그리기를 할 대상인 랜더 타겟





	// 비트맵. 자원으로 분리되어야한다.
	ID2D1Bitmap* m_pBitmap;
};

