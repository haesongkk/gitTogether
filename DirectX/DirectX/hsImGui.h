#pragma once
#include "hsSystem.h"
class hsImGui : public hsSystem
{
public:
	hsImGui(HWND _hWnd, ID3D11Device* _pDevice, ID3D11DeviceContext* _pDC);

	virtual void Init() override;
	virtual void Update() override;
	virtual void Finalize() override;
};

