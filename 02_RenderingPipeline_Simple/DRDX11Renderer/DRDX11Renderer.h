#pragma once
#include "I3DRenderer.h"
#include <Windows.h>
//#include "GameTimer.h"		// Timer�ε� ���� �ܺο��� �޴´�. (���躯��)
#include "DX11Define.h"

#include "./FL/d3dx11Effect.h"	// Box, Skull
#include "struct_and_const.h"	// Box, Skull


class DXTKFont;
class DRCamera;
class Axis;
class Grid;
class MeshObject;
class Box;

/// <summary>
/// DirectX 11�������� ���� ��ī���� ������ �ٷշ�����
/// 
/// 2021.02.14 LeHideOffice
/// </summary>
class DRDX11Renderer : public I3DRenderer
{
public:
	DRDX11Renderer();
	~DRDX11Renderer();

	// ������ �ʱ�ȭ�Ѵ�.
	virtual bool Initialize(int hinst, int hWnd, int screenWidth, int screenHeight) override;

	// ������Ʈ
	virtual void Update(float deltaTime) override;

	// ������ ����
	virtual void BeginRender() override;

	// ������ ���� �� �׽�Ʈ �Լ�
private:
	virtual void Draw_Test() override;

private:
	void OnResize();

public:
	// ������ ��
	virtual void EndRender() override;

	// ������ �����Ѵ�.
	virtual void Finalize() override;



	// ȭ�鿡 ����� ���� ���
private:
	void Draw_Status();

	float GetAspectRatio() const;


	// �ð� ����
private:
	float m_deltaTime;


	/// DirectX 11 ���� ���

	// �ʱ�ȭ
private:
	//HINSTANCE mhAppInst;
	HWND      mhMainWnd;				/// ���� �ڵ�
	bool      mAppPaused;
	bool      mMinimized;
	bool      mMaximized;
	bool      mResizing;				/// �̷� �͵��� ���ؼ� Win32App���� ���� ���� Ŭ������ ����� �� ���������.
	UINT      m4xMsaaQuality;


	ID3D11Device* md3dDevice;						// D3D11 ����̽�
	ID3D11DeviceContext* md3dImmediateContext;		// ����̽� ���ؽ�Ʈ
	IDXGISwapChain* mSwapChain;						// ����ü��
	ID3D11Texture2D* mDepthStencilBuffer;			// ���� ���Ľ� ����
	ID3D11RenderTargetView* mRenderTargetView;		// ���� Ÿ�� ��
	ID3D11DepthStencilView* mDepthStencilView;		// ���� ���Ľ� ��
	D3D11_VIEWPORT mScreenViewport;					// ����Ʈ


	/// ���� WinMain���� ���� ���� ������ ���� ���� ���� ������.
	// Derived class should set these in derived constructor to customize starting values.
	std::wstring mMainWndCaption;		/// �̷����̳�
	D3D_DRIVER_TYPE md3dDriverType;

public:
	int mClientWidth;					///
	int mClientHeight;					/// �̷� �͵�

private:
	bool mEnable4xMsaa;


	// �ʱ�ȭ ���� ����ü��
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
	// �̸� ���� ��Ʈ�� �����ΰ� ����Ī�Ѵ�.
private:
	ID3D11RasterizerState* mWireframeRS;
	ID3D11RasterizerState* mSolidRS;

	// ��Ʈ������ �������Ľ� ������Ʈ�� ������ �ƴ�.
	ID3D11DepthStencilState* NormalDSS;


private:
	void CreateRenderStates();


	/// Axis�� Grid�� helper�׸���
private:
	Axis* m_WorldAxes;
	Grid* m_Grid;


	/// ����� ����
private:
	DXGI_ADAPTER_DESC1 m_AdapterDesc1;
	HRESULT GetAdapterInfo();


	/// ���ÿ� �ڽ�
private:
	Box* m_TestBox;

};
