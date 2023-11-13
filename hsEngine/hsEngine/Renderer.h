#pragma once
#include "Base.h"

struct ID3D11Device;
struct ID3D11DeviceContext;
struct IDXGISwapChain;
struct ID3D11RenderTargetView;
struct ID3D11DepthStencilView;

class RenderComp;

class Renderer : public SingletonBase<Renderer>
{
public:
	void Create(HINSTANCE _hInst, int _width, int _height);
	void Run();
	void AddRenderComponent(shared_ptr<RenderComp> _spRenderComp);
	void DeleteRenderComponent(shared_ptr<RenderComp> _spRenderComp);
private:
	HWND m_hWnd;
	WNDCLASSEXW m_wcex;

	UINT m_width = 1024;
	UINT m_height = 768;

	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDeviceContext;
	IDXGISwapChain* m_pSwapChain;
	ID3D11RenderTargetView* m_pRenderTargetView;
	ID3D11DepthStencilView* m_pDepthStencilView;
	
	wpvector<RenderComp> m_renderQ;
};

