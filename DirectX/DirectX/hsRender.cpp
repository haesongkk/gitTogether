#include "framework.h"
#include "hsRender.h"
#include "Helper.h"

hsRender::hsRender(HWND _hWnd, UINT _width, UINT _height)
{
    DXGI_SWAP_CHAIN_DESC swapDesc = {};
    swapDesc.BufferCount = 1;
    swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapDesc.OutputWindow = _hWnd;
    swapDesc.Windowed = true;
    swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapDesc.BufferDesc.Width = _width;
    swapDesc.BufferDesc.Height = _height;
    swapDesc.BufferDesc.RefreshRate.Numerator = 60;
    swapDesc.BufferDesc.RefreshRate.Denominator = 1;
    swapDesc.SampleDesc.Count = 1;
    swapDesc.SampleDesc.Quality = 0;

    UINT creationFlags = 0;

    D3D11CreateDeviceAndSwapChain(
        NULL, 
        D3D_DRIVER_TYPE_HARDWARE, 
        NULL, 
        creationFlags,
        NULL,
        NULL,
        D3D11_SDK_VERSION,
        &swapDesc, 
        &m_pSwapChain, 
        &m_pDevice, 
        NULL, 
        &m_pDeviceContext);

    assert(m_pSwapChain);
    assert(m_pDevice);
    assert(m_pDeviceContext);

    ID3D11Texture2D* pBackBufferTexture = nullptr;

    m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBufferTexture);
    assert(pBackBufferTexture);

    m_pDevice->CreateRenderTargetView(pBackBufferTexture, NULL, &m_pRenderTargetView);
    assert(m_pRenderTargetView);

    Helper::SafeRelease(pBackBufferTexture);

    m_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, NULL);

    D3D11_VIEWPORT viewport = {};
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = _width;
    viewport.Height = _height;
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    m_pDeviceContext->RSSetViewports(1, &viewport);

    D3D11_TEXTURE2D_DESC descDepth = {};
    descDepth.Width = _width;
    descDepth.Height = _height;
    descDepth.MipLevels = 1;
    descDepth.ArraySize = 1;
    descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    descDepth.SampleDesc.Count = 1;
    descDepth.SampleDesc.Quality = 0;
    descDepth.Usage = D3D11_USAGE_DEFAULT;
    descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    descDepth.CPUAccessFlags = 0;
    descDepth.MiscFlags = 0;

    ID3D11Texture2D* textureDepthStencil = nullptr;
    m_pDevice->CreateTexture2D(&descDepth, nullptr, &textureDepthStencil);
    assert(textureDepthStencil);

    D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
    descDSV.Format = descDepth.Format;
    descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    descDSV.Texture2D.MipSlice = 0;
    m_pDevice->CreateDepthStencilView(textureDepthStencil, &descDSV, &m_pDepthStencilView);
    assert(m_pDepthStencilView);
    Helper::SafeRelease(textureDepthStencil);

    m_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);

    m_camera.ratio = _width / (FLOAT)_height;
}

void hsRender::Init()
{
}

void hsRender::Update()
{
    if (m_camera.nearZ <= 0.0001f) { m_camera.nearZ = 0.0001f; }
    if (m_camera.nearZ >= 9.9f) { m_camera.nearZ = 9.9f; }
    if (m_camera.fovY <= 0.f) { m_camera.fovY = 0.01; }

    m_camera.viewMatrix = DirectX::XMMatrixLookToLH(
        m_camera.pos, 
        m_camera.dir, 
        m_camera.headDir);

    m_camera.projMatrix = DirectX::XMMatrixPerspectiveFovLH(
        m_camera.fovY,
        m_camera.ratio, 
        m_camera.nearZ,
        m_camera.farZ);

    m_pSwapChain->Present(0, 0);
    m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView, Color{ 0.0f, 0.3f, 0.5f, 1.0f });
    m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void hsRender::Finalize()
{
    Helper::SafeRelease(m_pDevice);
    Helper::SafeRelease(m_pDeviceContext);
    Helper::SafeRelease(m_pSwapChain);
    Helper::SafeRelease(m_pRenderTargetView);
    Helper::SafeRelease(m_pDepthStencilView);
}
