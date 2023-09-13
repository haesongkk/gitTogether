#include "framework.h"
#include "Renderer.h"
#include "Helper.h"

struct ConstantBuffer
{
    Matrix mWorld;
    Matrix mView;
    Matrix mProjection;
};

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

    InitWindow();
    InitDX();
    InitScene();

    vector<Vertex> vertices =
    {
        { Vector3(-1.0f, 1.0f, -1.0f),  Vector4(0.0f, 0.0f, 1.0f, 1.0f) },
        { Vector3(1.0f, 1.0f, -1.0f),   Vector4(0.0f, 1.0f, 0.0f, 1.0f) },
        { Vector3(1.0f, 1.0f, 1.0f),    Vector4(0.0f, 1.0f, 1.0f, 1.0f) },
        { Vector3(-1.0f, 1.0f, 1.0f),   Vector4(1.0f, 0.0f, 0.0f, 1.0f) },
        { Vector3(-1.0f, -1.0f, -1.0f), Vector4(1.0f, 0.0f, 1.0f, 1.0f) },
        { Vector3(1.0f, -1.0f, -1.0f),  Vector4(1.0f, 1.0f, 0.0f, 1.0f) },
        { Vector3(1.0f, -1.0f, 1.0f),   Vector4(1.0f, 1.0f, 1.0f, 1.0f) },
        { Vector3(-1.0f, -1.0f, 1.0f),  Vector4(0.0f, 0.0f, 0.0f, 1.0f) },
    };
    vector<WORD> indicies =
    {
        3, 1, 0,
        2, 1, 3,
        0, 5, 4,
        1, 5, 0,
        3, 4, 7,
        0, 4, 3,
        1, 6, 5,
        2, 6, 1,
        2, 7, 6,
        3, 7, 2,
        6, 4, 5,
        7, 4, 6,
    };
    
    m_objects.push_back(new Object);
    m_objects.push_back(new Object);
    m_objects.push_back(new Object);
    
    for (auto obj : m_objects)
    {
        obj->GetVertices() = vertices;
        int nVertices = obj->GetVertices().size();
        Vertex* vertices = new Vertex[nVertices];
        for (int i = 0; i < nVertices; i++)
            vertices[i] = obj->GetVertices()[i];
        
        D3D11_BUFFER_DESC vbDesc = {};
        vbDesc.ByteWidth = sizeof(Vertex) * nVertices;
        vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        vbDesc.Usage = D3D11_USAGE_DEFAULT;
        D3D11_SUBRESOURCE_DATA vbData = {};
        vbData.pSysMem = vertices;
        m_pDevice->CreateBuffer(&vbDesc, &vbData, &(obj->GetVB()));
        assert(obj->GetVB());
        Helper::SafeDeleteArray(vertices);

        obj->GetIndicies() = indicies;
        int nIndices = obj->GetIndicies().size();
        WORD* indices = new WORD[nIndices];
        for (int j = 0; j < nIndices; j++)
            indices[j] = obj->GetIndicies()[j];

        D3D11_BUFFER_DESC ibDesc = {};
        ibDesc.ByteWidth = sizeof(WORD) * nIndices;
        ibDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
        ibDesc.Usage = D3D11_USAGE_DEFAULT;
        D3D11_SUBRESOURCE_DATA ibData = {};
        ibData.pSysMem = indices;
        m_pDevice->CreateBuffer(&ibDesc, &ibData, &(obj->GetIB()));
        assert(obj->GetIB());
        Helper::SafeDeleteArray(indices);
    }

    vertices.clear();
    indicies.clear();

    m_objects[0]->GetPos() = { 0,0,0 };
    m_objects[1]->GetPos() = { 4,0,0 };
    m_objects[2]->GetPos() = { 4,0,0 };

    m_objects[0]->GetScale() = { 0.8,0.8,0.8 };
    m_objects[1]->GetScale() = { 0.3,0.3,0.3 };
    m_objects[2]->GetScale() = { 0.3,0.3,0.3 };

    m_objects[0]->GetParentObject() = nullptr;
    m_objects[1]->GetParentObject() = m_objects[0];
    m_objects[2]->GetParentObject() = m_objects[1];
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
    static double rot = 0;
    rot += 0.0001;
    m_objects[0]->GetRotate().y += 0.0001;
    m_objects[1]->GetRotate().y += 0.0001;
    m_objects[2]->GetRotate().y += 0.0001;

    for (auto obj : m_objects)
    {
        XMMATRIX mScale = XMMatrixScaling(obj->GetScale().x, obj->GetScale().y, obj->GetScale().z);
        XMMATRIX mRot = XMMatrixRotationX(obj->GetRotate().x) 
                        * XMMatrixRotationY(obj->GetRotate().y) 
                        * XMMatrixRotationZ(obj->GetRotate().z);
        XMMATRIX mTrans = XMMatrixTranslation(obj->GetPos().x, obj->GetPos().y, obj->GetPos().z);
        XMMATRIX mBasis = XMMatrixIdentity();
        if (obj->GetParentObject()) mBasis = obj->GetParentObject()->GetMatrix();
        obj->GetMatrix() = mScale * mRot * mTrans * mBasis;
    }
}

void Renderer::Render()
{
    m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView, Color{ 0.0f, 0.3f, 0.5f, 1.0f });
    m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
    m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    m_pDeviceContext->IASetInputLayout(m_pInputLayout);
    m_pDeviceContext->VSSetShader(m_pVertexShader, nullptr, 0);
    m_pDeviceContext->PSSetShader(m_pPixelShader, nullptr, 0);
    m_pDeviceContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);

    ConstantBuffer cb;
    cb.mView = XMMatrixTranspose(m_viewMatrix);
    cb.mProjection = XMMatrixTranspose(m_projMatrix);

    for (const auto& obj : m_objects)
    {
        cb.mWorld = XMMatrixTranspose(obj->GetMatrix());
        m_pDeviceContext->UpdateSubresource(m_pConstantBuffer, 0, nullptr, &cb, 0, 0);

        m_pDeviceContext->IASetVertexBuffers(0, 1, &(obj->GetVB()), &m_VertextBufferStride, &m_VertextBufferOffset);
        m_pDeviceContext->IASetIndexBuffer(obj->GetIB(), DXGI_FORMAT_R16_UINT, 0);

        m_pDeviceContext->DrawIndexed(obj->GetIndicies().size(), 0, 0);
    }

    m_pSwapChain->Present(0, 0);
}

void Renderer::Final()
{
    for (auto& obj : m_objects)
    {
        Helper::SafeRelease(obj->GetIB());
        Helper::SafeRelease(obj->GetVB());
    }
    Helper::SafeRelease(m_pDevice);
    Helper::SafeRelease(m_pDeviceContext);
    Helper::SafeRelease(m_pSwapChain);
    Helper::SafeRelease(m_pRenderTargetView);
    Helper::SafeRelease(m_pDepthStencilView);

    Helper::SafeRelease(m_pVertexShader);
    Helper::SafeRelease(m_pPixelShader);
    Helper::SafeRelease(m_pInputLayout);
    Helper::SafeRelease(m_pConstantBuffer);

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
    DXGI_SWAP_CHAIN_DESC swapDesc = {};
    swapDesc.BufferCount = 1;
    swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapDesc.OutputWindow = m_hWnd;	
    swapDesc.Windowed = true;
    swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapDesc.BufferDesc.Width = m_width;
    swapDesc.BufferDesc.Height = m_height;
    swapDesc.BufferDesc.RefreshRate.Numerator = 60;
    swapDesc.BufferDesc.RefreshRate.Denominator = 1;
    swapDesc.SampleDesc.Count = 1;
    swapDesc.SampleDesc.Quality = 0;

    UINT creationFlags = 0;

    D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, creationFlags, NULL, NULL,
        D3D11_SDK_VERSION, &swapDesc, &m_pSwapChain, &m_pDevice, NULL, &m_pDeviceContext);
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
    viewport.Width = (float)m_width;
    viewport.Height = (float)m_height;
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    m_pDeviceContext->RSSetViewports(1, &viewport);

    D3D11_TEXTURE2D_DESC descDepth = {};
    descDepth.Width = m_width;
    descDepth.Height = m_height;
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

}

void Renderer::InitScene()
{
    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };

    DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
    ID3DBlob* vertexShaderBuffer = nullptr;
    ID3DBlob* pixelShaderBuffer = nullptr;

    D3DCompileFromFile(L"BasicVertexShader.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "vs_4_0",
        dwShaderFlags, 0, &vertexShaderBuffer, nullptr);
    assert(vertexShaderBuffer);

    m_pDevice->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(),
        vertexShaderBuffer->GetBufferSize(), NULL, &m_pVertexShader);
    assert(m_pVertexShader);

    m_pDevice->CreateInputLayout(layout, ARRAYSIZE(layout),
        vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &m_pInputLayout);
    assert(m_pInputLayout);

    D3DCompileFromFile(L"BasicPixelShader.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "ps_4_0",
        dwShaderFlags, 0, &pixelShaderBuffer, nullptr);
    assert(pixelShaderBuffer);

    m_pDevice->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(),
        pixelShaderBuffer->GetBufferSize(), NULL, &m_pPixelShader);
    assert(m_pPixelShader);


    D3D11_BUFFER_DESC cbDesc = {};
    cbDesc.Usage = D3D11_USAGE_DEFAULT;
    cbDesc.ByteWidth = sizeof(ConstantBuffer);
    cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    cbDesc.CPUAccessFlags = 0;
    m_pDevice->CreateBuffer(&cbDesc, nullptr, &m_pConstantBuffer);
    assert(m_pConstantBuffer);

    Helper::SafeRelease(vertexShaderBuffer);
    Helper::SafeRelease(pixelShaderBuffer);

    m_VertextBufferStride = sizeof(Vertex);
    m_VertextBufferOffset = 0;

    XMVECTOR Eye = XMVectorSet(0.0f, 1.0f, -5.0f, 0.0f);
    XMVECTOR At = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
    XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
    m_viewMatrix = XMMatrixLookAtLH(Eye, At, Up);

    float fovY = XM_PIDIV2; 
    float nearZ = 0.01f;    
    float farZ = 100.0f;    
    m_projMatrix = XMMatrixPerspectiveFovLH(fovY, m_width / (FLOAT)m_height, nearZ, farZ);

}