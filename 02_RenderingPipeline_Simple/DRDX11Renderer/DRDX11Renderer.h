#pragma once
#include "I3DRenderer.h"
#include <Windows.h>
//#include "GameTimer.h"		// Timer인데 이제 외부에서 받는다. (설계변경)
#include "DX11Define.h"

#include "./FL/d3dx11Effect.h"	// Box, Skull
#include "struct_and_const.h"	// Box, Skull


class DXTKFont;
class DRCamera;
class Axis;
class Grid;
class MeshObject;
class Box;
class ObjLoader;

/// <summary>
/// DirectX 11버전으로 만든 아카데미 교육용 다롱랜더러
/// 
/// 2021.02.14 LeHideOffice
/// </summary>
class DRDX11Renderer : public I3DRenderer
{
public:
	DRDX11Renderer();
	~DRDX11Renderer();

	// 엔진을 초기화한다.
	virtual bool Initialize(int hinst, int hWnd, int screenWidth, int screenHeight) override;

	// 업데이트
	virtual void Update(float deltaTime) override;

	// 랜더링 시작
	virtual void BeginRender() override;

	// 랜더러 개발 중 테스트 함수
private:
	virtual void Draw_Test() override;

private:
	void OnResize();

public:
	// 랜더링 끝
	virtual void EndRender() override;

	// 엔진을 종료한다.
	virtual void Finalize() override;

	///
	ID3D11Device* GetDevice() { return md3dDevice; }

	// 화면에 디버깅 정보 출력
private:
	void Draw_Status();

	float GetAspectRatio() const;


	// 시간 관련
private:
	float m_deltaTime;


	/// DirectX 11 관련 멤버

	// 초기화
private:
	//HINSTANCE mhAppInst;
	HWND      mhMainWnd;				/// 윈도 핸들
	bool      mAppPaused;
	bool      mMinimized;
	bool      mMaximized;
	bool      mResizing;				/// 이런 것들을 위해서 Win32App등의 윈도 관련 클래스를 만들면 더 깔끔해진다.
	UINT      m4xMsaaQuality;


	ID3D11Device* md3dDevice;						// D3D11 디바이스
	ID3D11DeviceContext* md3dImmediateContext;		// 디바이스 컨텍스트
	IDXGISwapChain* mSwapChain;						// 스왑체인
	ID3D11Texture2D* mDepthStencilBuffer;			// 뎁스 스탠실 버퍼
	ID3D11RenderTargetView* mRenderTargetView;		// 랜더 타겟 뷰
	ID3D11DepthStencilView* mDepthStencilView;		// 뎁스 스탠실 뷰
	D3D11_VIEWPORT mScreenViewport;					// 뷰포트


	/// 봐서 WinMain으로 보낼 것은 보내고 여기 남길 것은 남기자.
	// Derived class should set these in derived constructor to customize starting values.
	std::wstring mMainWndCaption;		/// 이런것이나
	D3D_DRIVER_TYPE md3dDriverType;

public:
	int mClientWidth;					///
	int mClientHeight;					/// 이런 것들

private:
	bool mEnable4xMsaa;


	// 초기화 관련 구조체들
	D3D_FEATURE_LEVEL featureLevel;


	// DXTKFont
private:
	DXTKFont* m_pFont;

	// Camera
private:
	DRCamera* m_pCamera;

	// 	float mTheta;
	// 	float mPhi;
	// 	float mRadius;

	POINT mLastMousePos;

	virtual void OnMouseDown(int x, int y) override;
	virtual void OnMouseUp(int x, int y) override;
	virtual void OnMouseMove(int btnState, int x, int y) override;

	// Mesh (Box)
private:
	XMFLOAT4X4 mWorld;	// Transform Matrix
	XMFLOAT4X4 mView;
	XMFLOAT4X4 mProj;


	/// Render State
	// 미리 여러 세트를 만들어두고 스위칭한다.
private:
	ID3D11RasterizerState* mWireframeRS;
	ID3D11RasterizerState* mSolidRS;

	// 폰트때문에 뎁스스탠실 스테이트가 강제가 됐다.
	ID3D11DepthStencilState* NormalDSS;


private:
	void CreateRenderStates();


	/// Axis와 Grid등 helper그리기
private:
	Axis* m_WorldAxes;
	Grid* m_Grid;


	/// 어댑터 정보
private:
	DXGI_ADAPTER_DESC1 m_AdapterDesc1;
	HRESULT GetAdapterInfo();


	/// 예시용 박스
private:
	Box* m_TestBox;
	ObjLoader* m_Loader;

};

