#pragma once
#include "hsSystem.h"
class hsRenderSys : public hsSystem
{
public:
	hsRenderSys(HWND _hWnd, UINT _width, UINT _height);
	virtual void Init() override;
	virtual void Update() override;
	virtual void Finalize() override;

	ID3D11Device* GetDevice() { return m_pDevice; }
	ID3D11DeviceContext* GetDC() { return m_pDeviceContext; }

private:
	ID3D11Device* m_pDevice = nullptr;
	ID3D11DeviceContext* m_pDeviceContext = nullptr;
	IDXGISwapChain* m_pSwapChain = nullptr;
	ID3D11RenderTargetView* m_pRenderTargetView = nullptr;
	ID3D11DepthStencilView* m_pDepthStencilView = nullptr;

	struct Camera
	{
		Vector3 pos = { 0,3,-5 };
		Vector3 dir = { 0,-0.5,1 };
		Vector3 headDir = { 0,1,0 };
		Matrix viewMatrix;

		float fovY = 1.570796327f;
		float ratio = 1.5f;
		float nearZ = 0.01f;
		float farZ = 100.0f;
		Matrix projMatrix;
	}m_camera;
};

