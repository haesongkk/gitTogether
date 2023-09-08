#include "framework.h"
#include "Renderer.h"
#include "Helper.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

void Renderer::Init(HINSTANCE hInstance)
{
    m_inst = hInstance;
    SetVS(L"BasicVertexShader.hlsl");
    SetPS(L"BasicPixelShader.hlsl");
    SetnVertices(6);
    SetVertices(0, Vertex(Vector3(-0.5f, 0.5f, 0.5f), Vector4(1.0f, 0.0f, 0.0f, 1.0f)));
    SetVertices(1, Vertex(Vector3(0.5f, 0.5f, 0.5f), Vector4(0.0f, 1.0f, 0.0f, 1.0f)));
    SetVertices(2, Vertex(Vector3(-0.5f, -0.5f, 0.5f), Vector4(0.0f, 0.0f, 1.0f, 1.0f)));
    SetVertices(3, Vertex(Vector3(0.5f, -0.5f, 0.5f), Vector4(0.0f, 0.0f, 1.0f, 1.0f)));
    SetnIndices(6);
    SetIndicies(0, 0);
    SetIndicies(1, 1);
    SetIndicies(2, 2);
    SetIndicies(3, 2);
    SetIndicies(4, 1);
    SetIndicies(5, 3);

    InitWindow();
    InitDX();
    InitScene();
}

void Renderer::Run()
{
    while (TRUE)
    {
        MSG msg;
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT) break;

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            Update();
            Render();
        }
    }
}

void Renderer::Update()
{
}

void Renderer::Render()
{
    m_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, NULL);

    Color color(0.0f, 0.3f, 0.5f, 1.0f);
    m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView, color);
    m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    if (m_pVertexBuffer) 
        m_pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &m_VertextBufferStride, &m_VertextBufferOffset);

    if (m_pInputLayout) 
        m_pDeviceContext->IASetInputLayout(m_pInputLayout);

    if (m_pVertexShader) 
        m_pDeviceContext->VSSetShader(m_pVertexShader, nullptr, 0);

    if (m_pPixelShader) 
        m_pDeviceContext->PSSetShader(m_pPixelShader, nullptr, 0);

    if (m_pIndexBuffer) 
        m_pDeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);

    if (m_pVertexBuffer)
    {
        if (m_pIndexBuffer)
            m_pDeviceContext->DrawIndexed(m_nIndices, 0, 0);
        else 
            m_pDeviceContext->Draw(m_nVertices, 0);
    }

    m_pSwapChain->Present(0, 0);
}

void Renderer::Final()
{
    Helper::SafeRelease(m_pDevice);
    Helper::SafeRelease(m_pDeviceContext);
    Helper::SafeRelease(m_pSwapChain);
    Helper::SafeRelease(m_pRenderTargetView);
    Helper::SafeRelease(m_pVertexShader);
    Helper::SafeRelease(m_pPixelShader);
    Helper::SafeRelease(m_pInputLayout);
    Helper::SafeRelease(m_pVertexBuffer);
    Helper::SafeRelease(m_pIndexBuffer);

    Helper::SafeDeleteArray(m_vertices);
    Helper::SafeDeleteArray(m_indices);
}

void Renderer::InitWindow()
{
    m_wcex.hInstance = m_inst;
    m_wcex.cbSize = sizeof(WNDCLASSEX);
    m_wcex.style = CS_HREDRAW | CS_VREDRAW;
    m_wcex.lpfnWndProc = WndProc;
    m_wcex.cbClsExtra = 0;
    m_wcex.cbWndExtra = 0;
    m_wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    m_wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    m_wcex.lpszClassName = m_class;

    RegisterClassExW(&m_wcex);

    m_hWnd = CreateWindowW(m_class, m_title, WS_OVERLAPPEDWINDOW,
        100, 100, m_width, m_height, nullptr, nullptr, m_inst, nullptr);

    assert(m_hWnd != nullptr);

    ShowWindow(m_hWnd, SW_SHOW);
    UpdateWindow(m_hWnd);
}

void Renderer::InitDX()
{
    HRESULT hr = S_OK;

    DXGI_SWAP_CHAIN_DESC swapDesc = {};

    swapDesc.BufferCount = 1;
    //swapDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapDesc.OutputWindow = m_hWnd;	// 스왑체인 출력할 창 핸들 값.
    swapDesc.Windowed = true;		// 창 모드 여부 설정.
    swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    // 백버퍼(텍스처)의 가로/세로 크기 설정.
    swapDesc.BufferDesc.Width = m_width;
    swapDesc.BufferDesc.Height = m_height;
    // 화면 주사율 설정.
    swapDesc.BufferDesc.RefreshRate.Numerator = 60;
    swapDesc.BufferDesc.RefreshRate.Denominator = 1;
    // 샘플링 관련 설정.
    swapDesc.SampleDesc.Count = 1;
    swapDesc.SampleDesc.Quality = 0;


    UINT creationFlags = 0;
    creationFlags |= D3D11_CREATE_DEVICE_DEBUG;

    hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, creationFlags, NULL, NULL,
        D3D11_SDK_VERSION, &swapDesc, &m_pSwapChain, &m_pDevice, NULL, &m_pDeviceContext);
    assert(!FAILED(hr));

    ID3D11Texture2D* pBackBufferTexture = nullptr;

    hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBufferTexture);
    assert(!FAILED(hr));

    hr = m_pDevice->CreateRenderTargetView(pBackBufferTexture, NULL, &m_pRenderTargetView);
    assert(!FAILED(hr));

    pBackBufferTexture->Release();

    m_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, NULL);

    D3D11_VIEWPORT viewport = {};

    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = (float)m_width;
    viewport.Height = (float)m_height;
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;

    m_pDeviceContext->RSSetViewports(1, &viewport);
}

void Renderer::InitScene()
{
    DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
    ID3DBlob* vertexShaderBuffer = nullptr;
    ID3DBlob* pixelShaderBuffer = nullptr;

    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };

    if (m_vsFileName != nullptr)
    {
        D3DCompileFromFile(m_vsFileName, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "vs_4_0",
            dwShaderFlags, 0, &vertexShaderBuffer, nullptr);
        assert(vertexShaderBuffer != nullptr);

        m_pDevice->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(),
            vertexShaderBuffer->GetBufferSize(), NULL, &m_pVertexShader);
        assert(m_pVertexShader != nullptr);

        m_pDevice->CreateInputLayout(layout, ARRAYSIZE(layout),
            vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &m_pInputLayout);
        assert(m_pInputLayout != nullptr);

        vertexShaderBuffer->Release();
    }

    if (m_psFileName != nullptr)
    {
        D3DCompileFromFile(m_psFileName, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "ps_4_0",
            dwShaderFlags, 0, &pixelShaderBuffer, nullptr);
        assert(pixelShaderBuffer != nullptr);

        m_pDevice->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(),
            pixelShaderBuffer->GetBufferSize(), NULL, &m_pPixelShader);
        assert(m_pPixelShader != nullptr);

        pixelShaderBuffer->Release();
    }

    if (m_vertices != nullptr)
    {
        D3D11_BUFFER_DESC vbDesc = {};
        vbDesc.ByteWidth = sizeof(Vertex) * m_nVertices;
        vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        vbDesc.Usage = D3D11_USAGE_DEFAULT;

        D3D11_SUBRESOURCE_DATA vbData = {};
        vbData.pSysMem = m_vertices;	// 배열 데이터 할당

        m_pDevice->CreateBuffer(&vbDesc, &vbData, &m_pVertexBuffer);
        assert(m_pVertexBuffer != nullptr);

        m_VertextBufferStride = sizeof(Vertex);
        m_VertextBufferOffset = 0;


    }

    if (m_indices != nullptr)
    {

        D3D11_BUFFER_DESC ibDesc = {};
        ibDesc.ByteWidth = sizeof(WORD) * m_nIndices;
        ibDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
        ibDesc.Usage = D3D11_USAGE_DEFAULT;

        D3D11_SUBRESOURCE_DATA ibData = {};
        ibData.pSysMem = m_indices;


        m_pDevice->CreateBuffer(&ibDesc, &ibData, &m_pIndexBuffer);
        assert(m_pIndexBuffer != nullptr);

    }
}

void Renderer::SetPS(LPCWSTR _ps)
{
    m_psFileName = _ps;
}

void Renderer::SetVS(LPCWSTR _vs)
{
    m_vsFileName = _vs;
}

void Renderer::SetnVertices(UINT _nV)
{
    m_nVertices = _nV;
    m_vertices = new Vertex[_nV];
}

void Renderer::SetVertices(UINT _i, Vertex _v)
{
    m_vertices[_i] = _v;
}

void Renderer::SetnIndices(UINT _nI)
{
    m_nIndices = _nI;
    m_indices = new WORD[_nI];
}

void Renderer::SetIndicies(UINT _i, WORD _w)
{
    m_indices[_i] = _w;
}
