#pragma once
#include "hsSystem.h"
class hsUISys : public hsSystem
{
public:
	hsUISys(HWND _hWnd, ID3D11Device* _pDevice, ID3D11DeviceContext* _pDC);

	virtual void Init() override;
	virtual void Update() override;
	virtual void Finalize() override;
};

