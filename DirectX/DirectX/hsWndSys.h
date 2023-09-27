#pragma once
#include "hsSystem.h"
class hsWndSys : public hsSystem
{
public:
	hsWndSys(HINSTANCE _hInst, UINT _width, UINT _height);

	virtual void Init() override;
	virtual void Update() override;
	virtual void Finalize() override;

	HWND GetHwnd() { return m_hWnd; }
	MSG GetMsg() { return m_msg; }

private:
	WNDCLASSEXW m_wcex = {};

	HWND m_hWnd = {};
	MSG m_msg = {};

};

