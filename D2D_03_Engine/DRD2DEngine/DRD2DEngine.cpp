#include "DRD2DEngine.h"

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")	// writefactory
#pragma comment(lib, "windowscodecs.lib") // WIC


DRD2DEngine::DRD2DEngine()
	: m_hWnd(NULL), m_pD2DFactory(nullptr),
	m_pRenderTarget(nullptr),
	m_pDWriteFactory(NULL),
	m_pTextFormat(NULL),
	m_pWhiteBrush(NULL),
	m_txtIndex(0)
	//txt(nullptr)
{
	// TODO:멤버를 모두 초기화 해 놓을 것.
}

DRD2DEngine::~DRD2DEngine()
{
	// 명시적인 Finalize() 호출 이후에 엔진 소멸 시 필요한 처리
}


///
/// 폰트 바꾸고 싶음
/// CreateFactory
/// CreateHwndRenderTarget
/// QueryInterface
/// DwriteCreateFactory
/// CreateTextFormat
/// SetTextFormat
/// CreateSolidBrush
/// begin -> DrawText -> end
/// Release
/// 
void DRD2DEngine::Initialize(HINSTANCE hinst, HWND hWnd, int screenWidth, int screenHeight)
{
	/// Initializes the COM library on the current thread and identifies the concurrency model as single-thread apartment (STA).
	HRESULT _hResult = CoInitialize(NULL);
	//CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);

	m_hWnd = hWnd;

	HRESULT hr = S_OK;

	// 1. Direct2D Factory를 만든다.
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED,
		&m_pD2DFactory);

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

void DRD2DEngine::ChangeDisplay(int screenWidth, int screenHeight)
{

}

void DRD2DEngine::RestoreDisplay()
{

}

void DRD2DEngine::ClearScreen()
{

}

HRESULT DRD2DEngine::CreateDeviceResources()
{
	HRESULT hr = S_OK;

	// 2. Direct2D 랜더 타겟을 생성한다.
	if (m_pRenderTarget == nullptr)
	{
		RECT rc;
		GetClientRect(m_hWnd, &rc);

		D2D1_SIZE_U size = D2D1::SizeU(
			rc.right - rc.left,
			rc.bottom - rc.top);

		hr = m_pD2DFactory->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(m_hWnd, size),
			&m_pRenderTarget);

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

		// 나무 배경 그림 얜 jpg 인데도 잘 불러와지네
		if (SUCCEEDED(hr))
		{
			// Create a bitmap by loading it from a file.
			hr = LoadBitmapFromFile(
				m_pRenderTarget,
				m_pWICFactory,
				L".\\sampleImage.jpg",
				100,
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
				0,0,
				//size1.width,
				//size1.height,
				&m_pBitmap_Samurai
			);
		}

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
	}

	return hr;
}

HRESULT DRD2DEngine::LoadResourceBitmap(ID2D1RenderTarget* pRenderTarget, IWICImagingFactory* pIWICFactory, PCWSTR resourceName, PCWSTR resourceType, UINT destinationWidth, UINT destinationHeight, ID2D1Bitmap** ppBitmap)
{
	HRESULT hr = S_OK;
	IWICBitmapDecoder* pDecoder = NULL;
	IWICBitmapFrameDecode* pSource = NULL;
	IWICStream* pStream = NULL;
	IWICFormatConverter* pConverter = NULL;
	IWICBitmapScaler* pScaler = NULL;

	HRSRC imageResHandle = NULL;
	HGLOBAL imageResDataHandle = NULL;
	void* pImageFile = NULL;
	DWORD imageFileSize = 0;

	// Locate the resource.
	imageResHandle = FindResourceW(HINST_THISCOMPONENT, resourceName, resourceType);

	hr = imageResHandle ? S_OK : E_FAIL;
	if (SUCCEEDED(hr))
	{
		// Load the resource.
		imageResDataHandle = LoadResource(HINST_THISCOMPONENT, imageResHandle);

		hr = imageResDataHandle ? S_OK : E_FAIL;
	}
	if (SUCCEEDED(hr))
	{
		// Lock it to get a system memory pointer.
		pImageFile = LockResource(imageResDataHandle);

		hr = pImageFile ? S_OK : E_FAIL;
	}
	if (SUCCEEDED(hr))
	{
		// Calculate the size.
		imageFileSize = SizeofResource(HINST_THISCOMPONENT, imageResHandle);

		hr = imageFileSize ? S_OK : E_FAIL;
	}
	if (SUCCEEDED(hr))
	{
		// Create a WIC stream to map onto the memory.
		hr = pIWICFactory->CreateStream(&pStream);
	}
	if (SUCCEEDED(hr))
	{
		// Initialize the stream with the memory pointer and size.
		hr = pStream->InitializeFromMemory(
			reinterpret_cast<BYTE*>(pImageFile),
			imageFileSize
		);
	}
	if (SUCCEEDED(hr))
	{
		// Create a decoder for the stream.
		hr = pIWICFactory->CreateDecoderFromStream(
			pStream,
			NULL,
			WICDecodeMetadataCacheOnLoad,
			&pDecoder
		);
	}
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
		}
		else
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
		//create a Direct2D bitmap from the WIC bitmap.
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

HRESULT DRD2DEngine::LoadBitmapFromFile(ID2D1RenderTarget* pRenderTarget, IWICImagingFactory* pIWICFactory, PCWSTR uri, UINT destinationWidth, UINT destinationHeight, ID2D1Bitmap** ppBitmap)
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

void DRD2DEngine::Render()
{
	HRESULT hr = CreateDeviceResources();

	if (SUCCEEDED(hr) && !(m_pRenderTarget->CheckWindowState() & D2D1_WINDOW_STATE_OCCLUDED))
	{
		// Retrieve the size of the render target.
		D2D1_SIZE_F renderTargetSize = m_pRenderTarget->GetSize();

		///--------------------------------------------------
		/// 그리기는 BegineDraw()와 EndDraw()사이에
		m_pRenderTarget->BeginDraw();

		m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

		// 빨강 배경
		m_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::BlueViolet));

		/// 타원을 그려보자
		D2D1_ELLIPSE _testRegion;
		_testRegion.point.x = 200;
		_testRegion.point.y = 200;
		_testRegion.radiusX = 30.0f;
		_testRegion.radiusY = 50.0f;

		ID2D1SolidColorBrush* pBrownBrush = NULL;
		m_pRenderTarget->CreateSolidColorBrush(ColorF(0.80f, 0.40f, 0.f), &pBrownBrush);

		ID2D1SolidColorBrush* pBlueBrush = NULL;
		m_pRenderTarget->CreateSolidColorBrush(ColorF(0.0f, 0.0f, 1.f), &pBlueBrush);

		// 노란 타원
		m_pRenderTarget->FillEllipse(_testRegion, pBrownBrush);


		D2D1_POINT_2F _point;
		_point.x = 100.0f;
		_point.y = 100.0f;
		D2D1_POINT_2F _point2;
		_point2.x = 300.0f;
		_point2.y = 300.0f;

		// 파란 사선
		m_pRenderTarget->DrawLine(_point, _point2, pBlueBrush, 2.0f);


		D2D1_SIZE_F size = m_pBitmap->GetSize();
		D2D1_SIZE_F size1 = m_pBitmap_Samurai->GetSize();
		//D2D1_SIZE_U size1 = m_pBitmap_Samurai->GetPixelSize();

		// Draw a bitmap in the upper-left corner of the window.
		/*m_pRenderTarget->DrawBitmap(
			m_pBitmap,
			D2D1::RectF(0.0f, 0.0f, size.width, size.height)
		);*/

		// Samurai 근데 화질 개구리게 나옴
		m_pRenderTarget->DrawBitmap(
			m_pBitmap_Samurai,
			D2D1::RectF(0.f, 0.0f, size1.width, size1.height)
		);

		// 다쓰면 해제할 것
		pBrownBrush->Release();
		pBrownBrush = NULL;


		static const WCHAR sc_helloWorld[] = L"재미있는 색칠공부";

		// 안녕 비정한 세상아 출력
		m_pRenderTarget->DrawText(
			sc_helloWorld,
			ARRAYSIZE(sc_helloWorld) - 1,
			m_pTextFormat,
			D2D1::RectF(0, 0, renderTargetSize.width, renderTargetSize.height),
			m_pWhiteBrush
		);


		ID2D1SolidColorBrush* pTextBrush = NULL;
		m_pRenderTarget->CreateSolidColorBrush(ColorF(D2D1::ColorF::Turquoise), &pTextBrush);


		for (int i = 0; i < m_vecTxt.size(); i++)
		{
			if (m_vecTxt[i] != nullptr)
			{
				// 되나?
				m_pRenderTarget->DrawText(
					m_vecTxt[i],
					m_vecRequiredSize[i],
					m_pTextFormat,
					D2D1::RectF(m_vecPosx[i], m_vecPosy[i], renderTargetSizeTxt.width, renderTargetSizeTxt.height),
					pTextBrush);
			}
		}

		//delete[] txt;


		// whiteBrush 얘는 매번 만드는 브러쉬가 아니라 해제는 마지막에 한번만 해도됨

		pTextBrush->Release();
		pTextBrush = NULL;

		//
		// Reset back to the identity transform
		//
		//m_pRenderTarget->SetTransform(
		//	D2D1::Matrix3x2F::Translation(0, renderTargetSize.height - 200));



		hr = m_pRenderTarget->EndDraw();
		/// 그리기는 BegineDraw()와 EndDraw()사이에
		///--------------------------------------------------

		if (hr == D2DERR_RECREATE_TARGET)
		{
			hr = S_OK;
			DiscardDeviceResources();
		}
	}
}


void DRD2DEngine::DiscardDeviceResources()
{
	SafeRelease(&m_pRenderTarget);
	SafeRelease(&m_pWhiteBrush);
}

void DRD2DEngine::Finalize()
{
	//delete[] txt;

	SafeRelease(&m_pD2DFactory);//m_pD2DFactory->Release();
	SafeRelease(&m_pRenderTarget);
	SafeRelease(&m_pDWriteFactory);
	SafeRelease(&m_pTextFormat);
	SafeRelease(&m_pWhiteBrush);

	CoUninitialize();
}

void DRD2DEngine::DrawRectangle(int x, int y, int x1, int y1, COLORREF crBrush)
{

}

void DRD2DEngine::DrawRectangle(int x, int y, int x1, int y1)
{

}

void DRD2DEngine::DrawLine(int, int, int, int)
{

}

void DRD2DEngine::DrawEllipse(int x, int y)
{

}

void DRD2DEngine::DrawEllipse(int left, int top, int right, int bottom)
{

}

void DRD2DEngine::DrawSelectRectangle(int x1, int y1, int x2, int y2)
{

}

void DRD2DEngine::DrawSelectedCircle(int x, int y, int size)
{

}

bool DRD2DEngine::DrawText(int x, int y, const char* pch, ...)
{
	//renderTargetSizeTxt = m_pRenderTarget->GetSize();

	requiredSize = MultiByteToWideChar(CP_ACP, 0, pch, strlen(pch), NULL, NULL);
	m_vecRequiredSize.push_back(requiredSize);

	if (requiredSize == 0) 
	{
		std::cerr << "Failed to get required buffer size." << std::endl;
		return 1;
	}

	WCHAR* txt = new WCHAR[requiredSize];
	//WCHAR Ltext[requiredSize] = {0,};
	

	//LPCWSTR Ltext;


	if (MultiByteToWideChar(CP_ACP, 0, pch, strlen(pch), txt, requiredSize) == 0) {
		std::cerr << "Failed to convert the string to wide characters." << std::endl;
		//delete[] txt;
		return 1;
	}

	// 변환된 와이드 문자열 사용
	//std::wcout << txt << std::endl;
	m_vecTxt.push_back(txt);

	m_vecPosx.push_back(x);
	m_vecPosy.push_back(y);

	//m_txtIndex++;

	//delete[] txt;


	/*m_pRenderTarget->DrawText(
		txt,
		requiredSize,
		m_pTextFormat,
		D2D1::RectF(x, y, renderTargetSizeTxt.width, renderTargetSizeTxt.height),
		m_pWhiteBrush);*/

	//txt = pch;
	//posx = x;
	//posy = y;


	//// 안녕 비정한 세상아 출력
	//m_pRenderTarget->DrawText(
	//	sc_helloWorld,
	//	ARRAYSIZE(sc_helloWorld) - 1,
	//	m_pTextFormat,
	//	D2D1::RectF(0, 0, renderTargetSize.width, renderTargetSize.height),
	//	m_pWhiteBrush
	//);

	

	return S_OK;
}

int DRD2DEngine::LoadSpriteSheet(char* filePath)
{
	return 0;
}

DRSprite* DRD2DEngine::LoadSpriteFromSheet(char* filePath)
{
	return nullptr;
}

DRSprite* DRD2DEngine::LoadSpriteFromSheet(int sheetIndex, int oriX, int oriY, int width, int height, int delayFrame)
{
	return nullptr;
}

void DRD2DEngine::DrawSprite(int spriteIndex, int dstX, int dstY)
{

}

void DRD2DEngine::DrawSprite(DRSprite* sprite, int dstX, int dstY)
{

}

void DRD2DEngine::RemoveAllSprites()
{

}

void DRD2DEngine::RemoveAllSpriteSheets()
{

}
