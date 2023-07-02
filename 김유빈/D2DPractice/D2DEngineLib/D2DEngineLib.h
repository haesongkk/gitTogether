#pragma once
//#include "Macro.h"
#include <windows.h>
#include <d2d1.h>
#include <d2d1_1.h>
#include "d2d1helper.h"		// ColorF
#include <dwrite.h>			// writefactory
#include <wincodec.h>		// WIC

#include <iostream>
#include <string>
#include <vector>

template<class Interface>
inline void
SafeRelease(
	Interface** ppInterfaceToRelease
)
{
	if (*ppInterfaceToRelease != NULL)
	{
		(*ppInterfaceToRelease)->Release();

		(*ppInterfaceToRelease) = NULL;
	}
}

using namespace D2D1;
using namespace std;

class D2DEngineLib
{
public:
	D2DEngineLib();
	~D2DEngineLib();

public:
	void Initialize(HINSTANCE hinst, HWND hWnd, int screenWidth, int screenHeight);
	void ClearScreen();

	void BeginDraw();
	void EndDraw();
	void Finalize();

	// �ܺο��� �θ� �Լ�
	// ������ ��ǥ�� �簢���� ����Ѵ�. �̷������� ��ġ ���������� ���� �Ѵ�.
	void DrawRectangle(int x, int y, int x1, int y1, COLORREF crBrush);
	void DrawRectangle(int x, int y, int x1, int y1);
	void DrawLine(POINT _pt1, POINT _pt2, COLORREF crBrush);
	void DrawEllipse(int x, int y);
	void DrawEllipse(int left, int top, int right, int bottom, COLORREF crBrush);
	void DrawSelectRectangle(POINT pt1, POINT pt2, POINT pt3, POINT pt4, COLORREF crBrush);
	void DrawSelectedCircle(int x, int y, int size, COLORREF crBrush);

	// ȭ�鿡 ���ڸ� ���
	bool DrawText(int x, int y, const char* pch,int num, COLORREF crBrush ...);
	void DrawImage();

private:
	HRESULT CreateDeviceResources();
	void DiscardDeviceResources();
	HRESULT LoadBitmapFromFile(
		ID2D1RenderTarget* pRenderTarget,
		IWICImagingFactory* pIWICFactory,
		PCWSTR uri,
		UINT destinationWidth,
		UINT destinationHeight,
		ID2D1Bitmap** ppBitmap
	);
	HRESULT LoadResourceBitmap(
		ID2D1RenderTarget* pRenderTarget,
		IWICImagingFactory* pIWICFactory,
		PCWSTR resourceName,
		PCWSTR resourceType,
		UINT destinationWidth,
		UINT destinationHeight,
		ID2D1Bitmap** ppBitmap
	);

private:
	HWND m_hWnd;								// �׸��⸦ �� ���� (1����� ����)
	HRESULT hr;
	RECT rc;
	//
	// Device Independant Resources
	ID2D1Factory* m_pD2DFactory;				// D2D�� Factory�� �����ϸ鼭 �����Ѵ�.

	IWICImagingFactory* m_pWICFactory;

	IDWriteFactory* m_pDWriteFactory;
	IDWriteTextFormat* m_pTextFormat;
	IDWriteTextFormat* m_pTextFormat1;
	ID2D1SolidColorBrush* m_pWhiteBrush;
	ID2D1SolidColorBrush* m_pBlueBrush;
	ID2D1SolidColorBrush* m_pTempBrush;			// �׸��� �Լ� �������� ����� �뵵

	// Device Dependant Resources
	ID2D1HwndRenderTarget* m_pRenderTarget;		// �׸��⸦ �� ����� ���� Ÿ��

	D2D1_SIZE_F renderTargetSizeTxt;
	//WCHAR* txt;
	float posx;
	float posy;
	int requiredSize;

	vector<WCHAR*> m_vecTxt;
	vector<float> m_vecPosx;
	vector<float> m_vecPosy;
	vector<int> m_vecRequiredSize;


	int m_txtIndex;
	// ��Ʈ��. �ڿ����� �и��Ǿ���Ѵ�.
	ID2D1Bitmap* m_pBitmap;
	ID2D1Bitmap* m_pBitmap_Samurai;

};