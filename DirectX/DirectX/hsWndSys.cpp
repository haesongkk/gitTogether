#include "framework.h"
#include "hsWndSys.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam);
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

hsWndSys::hsWndSys(HINSTANCE _hInst, UINT _width, UINT _height)
{
    m_wcex.hInstance = _hInst;
    m_wcex.cbSize = sizeof(WNDCLASSEX);
    m_wcex.style = CS_HREDRAW | CS_VREDRAW;
    m_wcex.lpfnWndProc = WndProc;
    m_wcex.cbClsExtra = 0;
    m_wcex.cbWndExtra = 0;
    m_wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    m_wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    m_wcex.lpszClassName = L"DefaultWindowCalss";

    RegisterClassExW(&m_wcex);

    RECT rt = { 0, 0, _width, _height };
    AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, FALSE);

    m_hWnd = CreateWindowW(
        L"DefaultWindowCalss",
        L"GameApp",
        WS_OVERLAPPEDWINDOW,
        100,
        100,
        rt.right - rt.left,
        rt.bottom - rt.top,
        nullptr,
        nullptr,
        _hInst,
        nullptr);

    assert(m_hWnd != nullptr);

    ShowWindow(m_hWnd, SW_SHOW);
    UpdateWindow(m_hWnd);
}

void hsWndSys::Init()
{
}

void hsWndSys::Update()
{
    if (PeekMessage(&m_msg, NULL, 0, 0, PM_REMOVE))
    {
        //if (msg.message == WM_QUIT) break;
        TranslateMessage(&m_msg);
        DispatchMessage(&m_msg);
    }
}

void hsWndSys::Finalize()
{
}
