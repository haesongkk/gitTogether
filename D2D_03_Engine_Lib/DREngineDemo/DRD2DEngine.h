#pragma once

/// Direct2d �׷��Ƚ� ����
///
/// �������̽��� ���ؼ�, WinAPI �׷��Ƚ� ������ ��ü �����ϴ�.
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
	/// �ʱ�ȭ ����

	// �ʱ�ȭ ����
	virtual void Initialize(HINSTANCE hinst, HWND hWnd, int screenWidth, int screenHeight) override;
	virtual void ChangeDisplay(int screenWidth, int screenHeight) override;
	virtual void RestoreDisplay() override;
	virtual void ClearScreen() override;

	// ȭ�鿡 �׸��⸦ �Ѵ�.
	virtual void Render() override;

	void DiscardDeviceResources();

	// ������ �����Ѵ�.
	virtual void Finalize() override;

	///--------------------------------------------------------------------------------
	/// �׸��� ����

	// ������ ��ǥ�� �簢���� ����Ѵ�. �̷������� ��ġ ���������� ���� �Ѵ�.
	virtual void DrawRectangle(int x, int y, int x1, int y1, COLORREF crBrush) override;
	virtual void DrawRectangle(int x, int y, int x1, int y1) override;
	virtual void DrawLine(int, int, int, int) override;
	virtual void DrawEllipse(int x, int y) override;
	virtual void DrawEllipse(int left, int top, int right, int bottom) override;
	virtual void DrawSelectRectangle(int x1, int y1, int x2, int y2) override;
	virtual void DrawSelectedCircle(int x, int y, int size) override;

	// ȭ�鿡 ���ڸ� ���
	virtual bool DrawText(int x, int y, const char* pch, ...) override;


	/// ----------------------------------------
	/// �׸��� ���� Sprite

	// ��Ʈ���� �ε��ϴ� ����
	virtual int LoadSpriteSheet(char* filePath) override;
	virtual DRSprite* LoadSpriteFromSheet(char* filePath) override;
	virtual DRSprite* LoadSpriteFromSheet(int sheetIndex, int oriX, int oriY, int width, int height, int delayFrame) override;


	// �ٷ� �̹��� ������ �ϳ� �д� ����
	///virtual DRSprite* LoadSprite(char* filePath) abstract;
	///virtual DRSprite* GetSprite(int index) abstract;


	// ��������Ʈ ���
	// ���� ��ġ ���� ������ ������ �����̸� ������ �ִ�.
	virtual void DrawSprite(int spriteIndex, int dstX, int dstY) override;
	virtual void DrawSprite(DRSprite* sprite, int dstX, int dstY) override;


	// ��� ��������Ʈ/��������Ʈ ��Ʈ ����
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
	HWND m_hWnd;								// �׸��⸦ �� ���� (1����� ����)

	// Device Independant Resources
	ID2D1Factory* m_pD2DFactory;				// D2D�� Factory�� �����ϸ鼭 �����Ѵ�.

	IWICImagingFactory* m_pWICFactory;

	IDWriteFactory* m_pDWriteFactory;
	IDWriteTextFormat* m_pTextFormat;
	ID2D1SolidColorBrush* m_pWhiteBrush;


	// Device Dependant Resources
	ID2D1HwndRenderTarget* m_pRenderTarget;		// �׸��⸦ �� ����� ���� Ÿ��





	// ��Ʈ��. �ڿ����� �и��Ǿ���Ѵ�.
	ID2D1Bitmap* m_pBitmap;
};

