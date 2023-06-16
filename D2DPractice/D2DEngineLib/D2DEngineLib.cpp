// D2DEngineLib.cpp : 정적 라이브러리를 위한 함수를 정의합니다.
//
#include "D2DEngineLib.h"
//#include "framework.h"

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")	// writefactory
#pragma comment(lib, "windowscodecs.lib") // WIC


D2DEngineLib::D2DEngineLib()
	: m_hWnd(NULL), m_pD2DFactory(nullptr),
	m_pRenderTarget(nullptr),
	m_pDWriteFactory(NULL),
	m_pTextFormat(NULL),
	m_pWhiteBrush(NULL),
	m_txtIndex(0)
{
}

D2DEngineLib::~D2DEngineLib()
{
}

void D2DEngineLib::Initialize(HINSTANCE hinst, HWND hWnd, int screenWidth, int screenHeight)
{
	HRESULT _hResult = CoInitialize(NULL);
	//CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);

	m_hWnd = hWnd;

	hr = S_OK;

	// 1. Direct2D Factory를 만든다.
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED,
		&m_pD2DFactory);

	/// 시스템 내부 폰트 및 크기 지정
	static const WCHAR msc_fontName[] = L"궁서";
	static const FLOAT msc_fontSize = 50;

	if (SUCCEEDED(hr))
	{
		// Create WIC factory.
		hr = CoCreateInstance(
			CLSID_WICImagingFactory,
			NULL,
			CLSCTX_INPROC_SERVER,
			IID_IWICImagingFactory,
			reinterpret_cast<void**>(&m_pWICFactory)
		);
	}

	if (SUCCEEDED(hr))
	{
		// Create a DirectWrite factory.
		hr = DWriteCreateFactory(
			DWRITE_FACTORY_TYPE_SHARED,
			__uuidof(m_pDWriteFactory),
			reinterpret_cast<IUnknown**>(&m_pDWriteFactory)
		);
	}

	if (SUCCEEDED(hr))
	{


		// Create a DirectWrite text format object.
		hr = m_pDWriteFactory->CreateTextFormat(
			msc_fontName,
			NULL/*이 값이 nullptr이면 시스템 폰트에서 첫 번째 인자로 주는 fontFamilyName을 검색한다.*/,
			DWRITE_FONT_WEIGHT_NORMAL,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			msc_fontSize,
			L"ko-KR", //locale
			&m_pTextFormat
		);
	}

	if (SUCCEEDED(hr))
	{
		// Center the text horizontally and vertically.
		m_pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);

		m_pTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

	}
}

void D2DEngineLib::ClearScreen()
{
	m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

	// 특정 색깔로 화면 비우기
	m_pRenderTarget->Clear(ColorF(ColorF::BlueViolet));
}

void D2DEngineLib::BeginDraw()
{
	hr = CreateDeviceResources();

	if (SUCCEEDED(hr) && !(m_pRenderTarget->CheckWindowState() & D2D1_WINDOW_STATE_OCCLUDED))
	{
		// Retrieve the size of the render target.
		D2D1_SIZE_F renderTargetSize = m_pRenderTarget->GetSize();

		///--------------------------------------------------
		/// 그리기는 BegineDraw()와 EndDraw()사이에
		m_pRenderTarget->BeginDraw();
	}
}

void D2DEngineLib::EndDraw()
{
	hr = m_pRenderTarget->EndDraw();
	/// 그리기는 BegineDraw()와 EndDraw()사이에
	///--------------------------------------------------

	if (hr == D2DERR_RECREATE_TARGET)
	{
		hr = S_OK;
		DiscardDeviceResources();
	}
}


void D2DEngineLib::DrawRectangle(int x, int y, int x1, int y1, COLORREF crBrush)
{
}

void D2DEngineLib::DrawRectangle(int x, int y, int x1, int y1)
{
}

void D2DEngineLib::DrawLine(int, int, int, int)
{
}

void D2DEngineLib::DrawEllipse(int x, int y)
{
}

void D2DEngineLib::DrawEllipse(int left, int top, int right, int bottom)
{
}

void D2DEngineLib::DrawImage()
{
	// 나무 배경 그림 얜 jpg 인데도 잘 불러와지네
	if (SUCCEEDED(hr))
	{
		// Create a bitmap by loading it from a file.
		hr = LoadBitmapFromFile(
			m_pRenderTarget,
			m_pWICFactory,
			L".\\sampleImage.jpg",
			0,
			0,
			&m_pBitmap
		);
	}

	// 사무라이
	if (SUCCEEDED(hr))
	{
		D2D1_SIZE_F size1 = m_pRenderTarget->GetSize();



		// Create a bitmap by loading it from a file.
		hr = LoadBitmapFromFile(
			m_pRenderTarget,
			m_pWICFactory,
			L".\\Images\\atk_1.bmp",
			0, 0,
			//size1.width,
			//size1.height,
			&m_pBitmap_Samurai
		);
	}
}

void D2DEngineLib::DrawSelectedCircle(int x, int y, int size)
{
}

bool D2DEngineLib::DrawText(int x, int y, const char* pch, ...)
{
	// 브러쉬 -> 안녕 비정한 세상아 출략
	if (SUCCEEDED(hr))
	{
		// Create a black brush.
		hr = m_pRenderTarget->CreateSolidColorBrush(
			D2D1::ColorF(D2D1::ColorF::Snow),
			&m_pWhiteBrush
		);
	}

	/*
	// Create a bitmap from an application resource.
	hr = LoadResourceBitmap(
		m_pRenderTarget,
		m_pWICFactory,
		L"SampleImage",
		L"Image",
		100,
		0,
		&m_pBitmap
	);
	*/



	/*
	if (SUCCEEDED(hr))
	{
		ID2D1GradientStopCollection* pGradientStops = NULL;
		// Create a linear gradient.
		static const D2D1_GRADIENT_STOP stops[] =
		{
			{   0.f,  { 0.f, 1.f, 1.f, 0.25f }  },
			{   1.f,  { 0.f, 0.f, 1.f, 1.f }  },
		};

		hr = m_pRenderTarget->CreateGradientStopCollection(
			stops,
			ARRAYSIZE(stops),
			&pGradientStops
		);
		if (SUCCEEDED(hr))
		{
			hr = m_pRenderTarget->CreateLinearGradientBrush(
				D2D1::LinearGradientBrushProperties(
					D2D1::Point2F(100, 0),
					D2D1::Point2F(100, 200)),
				D2D1::BrushProperties(),
				pGradientStops,
				&m_pLinearGradientBrush
			);
			pGradientStops->Release();
		}
	}

	// Create a bitmap from an application resource.
	hr = LoadResourceBitmap(
		m_pRenderTarget,
		m_pWICFactory,
		L"SampleImage",
		L"Image",
		100,
		0,
		&m_pBitmap
	);
	if (SUCCEEDED(hr))
	{
		// Create a bitmap by loading it from a file.
		hr = LoadBitmapFromFile(
			m_pRenderTarget,
			m_pWICFactory,
			L".\\sampleImage.jpg",
			100,
			0,
			&m_pAnotherBitmap
		);
	}
	if (SUCCEEDED(hr))
	{
		hr = CreateGridPatternBrush(m_pRenderTarget, &m_pGridPatternBitmapBrush);
	}
	*/
	return false;
}

HRESULT D2DEngineLib::CreateDeviceResources()
{
	HRESULT hr = S_OK;

	// 2. Direct2D 랜더 타겟을 생성한다.
	if (m_pRenderTarget == nullptr)
	{
		rc;
		GetClientRect(m_hWnd, &rc);

		D2D1_SIZE_U size = D2D1::SizeU(
			rc.right - rc.left,
			rc.bottom - rc.top);

		hr = m_pD2DFactory->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(m_hWnd, size),
			&m_pRenderTarget);

	}

	return hr;

	return E_NOTIMPL;
}

HRESULT D2DEngineLib::LoadBitmapFromFile(ID2D1RenderTarget* pRenderTarget, IWICImagingFactory* pIWICFactory, PCWSTR uri, UINT destinationWidth, UINT destinationHeight, ID2D1Bitmap** ppBitmap)
{
	HRESULT hr = S_OK;

	IWICBitmapDecoder* pDecoder = NULL;
	IWICBitmapFrameDecode* pSource = NULL;
	IWICStream* pStream = NULL;
	IWICFormatConverter* pConverter = NULL;
	IWICBitmapScaler* pScaler = NULL;

	hr = pIWICFactory->CreateDecoderFromFilename(
		uri,
		NULL,
		GENERIC_READ,
		WICDecodeMetadataCacheOnLoad,
		&pDecoder
	);
	if (SUCCEEDED(hr))
	{

		// Create the initial frame.
		hr = pDecoder->GetFrame(0, &pSource);
	}

	if (SUCCEEDED(hr))
	{
		// Convert the image format to 32bppPBGRA
		// (DXGI_FORMAT_B8G8R8A8_UNORM + D2D1_ALPHA_MODE_PREMULTIPLIED).
		hr = pIWICFactory->CreateFormatConverter(&pConverter);
	}
	if (SUCCEEDED(hr))
	{
		// If a new width or height was specified, create an
		// IWICBitmapScaler and use it to resize the image.
		if (destinationWidth != 0 || destinationHeight != 0)
		{
			UINT originalWidth, originalHeight;
			hr = pSource->GetSize(&originalWidth, &originalHeight);
			if (SUCCEEDED(hr))
			{
				if (destinationWidth == 0)
				{
					FLOAT scalar = static_cast<FLOAT>(destinationHeight) / static_cast<FLOAT>(originalHeight);
					destinationWidth = static_cast<UINT>(scalar * static_cast<FLOAT>(originalWidth));
				}
				else if (destinationHeight == 0)
				{
					FLOAT scalar = static_cast<FLOAT>(destinationWidth) / static_cast<FLOAT>(originalWidth);
					destinationHeight = static_cast<UINT>(scalar * static_cast<FLOAT>(originalHeight));
				}

				hr = pIWICFactory->CreateBitmapScaler(&pScaler);
				if (SUCCEEDED(hr))
				{
					hr = pScaler->Initialize(
						pSource,
						destinationWidth,
						destinationHeight,
						WICBitmapInterpolationModeCubic
					);
				}
				if (SUCCEEDED(hr))
				{
					hr = pConverter->Initialize(
						pScaler,
						GUID_WICPixelFormat32bppPBGRA,
						WICBitmapDitherTypeNone,
						NULL,
						0.f,
						WICBitmapPaletteTypeMedianCut
					);
				}
			}
		}
		else // Don't scale the image.
		{
			hr = pConverter->Initialize(
				pSource,
				GUID_WICPixelFormat32bppPBGRA,
				WICBitmapDitherTypeNone,
				NULL,
				0.f,
				WICBitmapPaletteTypeMedianCut
			);
		}
	}
	if (SUCCEEDED(hr))
	{
		// Create a Direct2D bitmap from the WIC bitmap.
		hr = pRenderTarget->CreateBitmapFromWicBitmap(
			pConverter,
			NULL,
			ppBitmap
		);
	}

	SafeRelease(&pDecoder);
	SafeRelease(&pSource);
	SafeRelease(&pStream);
	SafeRelease(&pConverter);
	SafeRelease(&pScaler);

	return hr;
}

void D2DEngineLib::DiscardDeviceResources()
{
	SafeRelease(&m_pRenderTarget);
	SafeRelease(&m_pWhiteBrush);
}

void D2DEngineLib::Finalize()
{
}