#include "framework.h"
#include "Renderer.h"
#include "Helper.h"

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


void Renderer::Init(HINSTANCE hInstance)
{
    InitWindow(hInstance);
    InitDX();
    InitScene();
    InitImGui();
    InitObj();
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
    UpdateScene();
    UpdateObject();
}

void Renderer::Render()
{
    RenderScene();
    RenderObject();
    RenderImGui();
}

void Renderer::Final()
{
    FinalImGui();
    FianlScene();
    FinalObject();
    FinalDX();
}

void Renderer::InitWindow(HINSTANCE hInstance)
{
    m_wcex.hInstance = hInstance;
    m_wcex.cbSize = sizeof(WNDCLASSEX);
    m_wcex.style = CS_HREDRAW | CS_VREDRAW;
    m_wcex.lpfnWndProc = WndProc;
    m_wcex.cbClsExtra = 0;
    m_wcex.cbWndExtra = 0;
    m_wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    m_wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    m_wcex.lpszClassName = L"DefaultWindowCalss";

    RegisterClassExW(&m_wcex);

    RECT rt = { 0, 0, (LONG)m_width, (LONG)m_height };
    AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, FALSE);

    m_hWnd = CreateWindowW(L"DefaultWindowCalss", L"GameApp", WS_OVERLAPPEDWINDOW,
        100, 100, rt.right - rt.left, rt.bottom - rt.top, nullptr, nullptr, hInstance, nullptr);

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
    // 마지막으로 주의를 기울여야 할 것은 버퍼에 데이터가 어떻게 배열되는지 알려주는 AlignedByteOffset 입니다. 
    // 이 레이아웃에서는 처음 12 byte를 위치 벡터에 사용하고 다음 16 byte를 색상으로 사용할 것임을 알려줘야 하는데, 
    // AlignedByteOffset이 각 요소가 어디서 시작하는지 보여줍니다. 
    // 여기서 직접 값을 입력하기보다 D3D11_APPEND_ALIGNED_ELEMENT 를 지정하여 자동으로 알아내도록 합니다. 
    // 나머지는 이 튜토리얼에서 크게 중요하지 않기 때문에 기본값으로 두었습니다.
    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TANGENT" , 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
    ID3DBlob* vertexShaderBuffer = nullptr;
    ID3DBlob* pixelShaderBuffer = nullptr;

    D3DCompileFromFile(L"./Shader/BasicVertexShader.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "vs_4_0",
        dwShaderFlags, 0, &vertexShaderBuffer, nullptr);
    assert(vertexShaderBuffer);

    D3DCompileFromFile(L"./Shader/BasicPixelShader.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "ps_4_0",
        dwShaderFlags, 0, &pixelShaderBuffer, nullptr);
    assert(pixelShaderBuffer);

    m_pDevice->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(),
        vertexShaderBuffer->GetBufferSize(), NULL, &m_pVertexShader);
    assert(m_pVertexShader);

    m_pDevice->CreateInputLayout(layout, ARRAYSIZE(layout),
        vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &m_pInputLayout);
    assert(m_pInputLayout);

    m_pDevice->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(),
        pixelShaderBuffer->GetBufferSize(), NULL, &m_pPixelShader);
    assert(m_pPixelShader);

    Helper::SafeRelease(vertexShaderBuffer);
    Helper::SafeRelease(pixelShaderBuffer);

    D3D11_BUFFER_DESC cbDesc = {};
    cbDesc.Usage = D3D11_USAGE_DEFAULT;
    cbDesc.ByteWidth = sizeof(TransformBuffer);
    cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    m_pDevice->CreateBuffer(&cbDesc, nullptr, &m_pTransformBuffer);
    assert(m_pTransformBuffer);

    cbDesc.Usage = D3D11_USAGE_DEFAULT;
    cbDesc.ByteWidth = sizeof(LightBuffer);
    cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    m_pDevice->CreateBuffer(&cbDesc, nullptr, &m_pLightBuffer);
    assert(m_pLightBuffer);

    cbDesc.Usage = D3D11_USAGE_DEFAULT;
    cbDesc.ByteWidth = sizeof(MarterialBuffer);
    cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    m_pDevice->CreateBuffer(&cbDesc, nullptr, &m_pMaterialBuffer);
    assert(m_pMaterialBuffer);

}

void Renderer::InitImGui()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    ImGui_ImplWin32_Init(m_hWnd);
    ImGui_ImplDX11_Init(this->m_pDevice, this->m_pDeviceContext);
}

void Renderer::InitObj()
{
    m_objects.push_back(new Object);

    for (auto obj : m_objects)
    {
        obj->GetVertices() =
        {
            { Vector3(-1.0f, 1.0f, -1.0f), Vector3(0.0f, 1.0f, 0.0f),  Vector2(1.0f, 0.0f) },
            { Vector3(1.0f, 1.0f, -1.0f),  Vector3(0.0f, 1.0f, 0.0f),  Vector2(0.0f, 0.0f) },
            { Vector3(1.0f, 1.0f, 1.0f),   Vector3(0.0f, 1.0f, 0.0f),  Vector2(0.0f, 1.0f) },
            { Vector3(-1.0f, 1.0f, 1.0f),  Vector3(0.0f, 1.0f, 0.0f),  Vector2(1.0f, 1.0f) },

            { Vector3(-1.0f, -1.0f, -1.0f),Vector3(0.0f, -1.0f, 0.0f), Vector2(0.0f, 0.0f) },
            { Vector3(1.0f, -1.0f, -1.0f), Vector3(0.0f, -1.0f, 0.0f), Vector2(1.0f, 0.0f) },
            { Vector3(1.0f, -1.0f, 1.0f),  Vector3(0.0f, -1.0f, 0.0f), Vector2(1.0f, 1.0f) },
            { Vector3(-1.0f, -1.0f, 1.0f), Vector3(0.0f, -1.0f, 0.0f), Vector2(0.0f, 1.0f) },

            { Vector3(-1.0f, -1.0f, 1.0f), Vector3(-1.0f, 0.0f, 0.0f), Vector2(0.0f, 1.0f) },
            { Vector3(-1.0f, -1.0f, -1.0f),Vector3(-1.0f, 0.0f, 0.0f), Vector2(1.0f, 1.0f) },
            { Vector3(-1.0f, 1.0f, -1.0f), Vector3(-1.0f, 0.0f, 0.0f), Vector2(1.0f, 0.0f) },
            { Vector3(-1.0f, 1.0f, 1.0f),  Vector3(-1.0f, 0.0f, 0.0f), Vector2(0.0f, 0.0f) },

            { Vector3(1.0f, -1.0f, 1.0f),  Vector3(1.0f, 0.0f, 0.0f),  Vector2(1.0f, 1.0f) },
            { Vector3(1.0f, -1.0f, -1.0f), Vector3(1.0f, 0.0f, 0.0f),  Vector2(0.0f, 1.0f) },
            { Vector3(1.0f, 1.0f, -1.0f),  Vector3(1.0f, 0.0f, 0.0f),  Vector2(0.0f, 0.0f) },
            { Vector3(1.0f, 1.0f, 1.0f),   Vector3(1.0f, 0.0f, 0.0f),  Vector2(1.0f, 0.0f) },

            { Vector3(-1.0f, -1.0f, -1.0f),Vector3(0.0f, 0.0f, -1.0f), Vector2(0.0f, 1.0f),Vector3(1.0f, 0.0f, 0.0f) },
            { Vector3(1.0f, -1.0f, -1.0f), Vector3(0.0f, 0.0f, -1.0f), Vector2(1.0f, 1.0f),Vector3(1.0f, 0.0f, 0.0f) },
            { Vector3(1.0f, 1.0f, -1.0f),  Vector3(0.0f, 0.0f, -1.0f), Vector2(1.0f, 0.0f),Vector3(1.0f, 0.0f, 0.0f) },
            { Vector3(-1.0f, 1.0f, -1.0f), Vector3(0.0f, 0.0f, -1.0f), Vector2(0.0f, 0.0f),Vector3(1.0f, 0.0f, 0.0f) },

            { Vector3(-1.0f, -1.0f, 1.0f), Vector3(0.0f, 0.0f, 1.0f),  Vector2(1.0f, 1.0f) },
            { Vector3(1.0f, -1.0f, 1.0f),  Vector3(0.0f, 0.0f, 1.0f),  Vector2(0.0f, 1.0f) },
            { Vector3(1.0f, 1.0f, 1.0f),   Vector3(0.0f, 0.0f, 1.0f),  Vector2(0.0f, 0.0f) },
            { Vector3(-1.0f, 1.0f, 1.0f),  Vector3(0.0f, 0.0f, 1.0f),  Vector2(1.0f, 0.0f) },
        };

        int nVertices = obj->GetVertices().size();
        Vertex* vertices = new Vertex[nVertices];
        for (int i = 0; i < nVertices; i++)
            vertices[i] = obj->GetVertices()[i];

        D3D11_BUFFER_DESC vbDesc = {};
        auto a = sizeof(Vertex);
        vbDesc.ByteWidth = sizeof(Vertex) * nVertices;
        vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        vbDesc.Usage = D3D11_USAGE_DEFAULT;
        D3D11_SUBRESOURCE_DATA vbData = {};
        vbData.pSysMem = vertices;
        m_pDevice->CreateBuffer(&vbDesc, &vbData, &(obj->GetVB()));
        assert(obj->GetVB());
        Helper::SafeDeleteArray(vertices);

        obj->GetIndicies() =
        {
            3,1,0,    2,1,3,
            6,4,5,    7,4,6,
            11,9,8,   10,9,11,
            14,12,13, 15,12,14,
            19,17,16, 18,17,19,
            22,20,21, 23,20,22
        };

        int nIndices = obj->GetIndicies().size();
        WORD* indices = new WORD[nIndices];
        for (int j = 0; j < nIndices; j++)
            indices[j] = obj->GetIndicies()[j];

        D3D11_BUFFER_DESC ibDesc = {};
        ibDesc.ByteWidth = sizeof(WORD) * nIndices;
        ibDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
        ibDesc.Usage = D3D11_USAGE_DEFAULT;
        ibDesc.CPUAccessFlags = 0;

        D3D11_SUBRESOURCE_DATA ibData = {};
        ibData.pSysMem = indices;

        m_pDevice->CreateBuffer(&ibDesc, &ibData, &(obj->GetIB()));
        assert(obj->GetIB());
        Helper::SafeDeleteArray(indices);

        obj->GetStride() = sizeof(Vertex);
        obj->GetOffset() = 0;

        CreateWICTextureFromFile(m_pDevice, L"./Texture/Bricks059_1K-JPG_Color.jpg", nullptr, &(obj->GetTRV()));
        CreateWICTextureFromFile(m_pDevice, L"./Texture/Bricks059_1K-JPG_NormalDX.jpg", nullptr, &(obj->GetNRV()));
        CreateWICTextureFromFile(m_pDevice, L"./Texture/Bricks059_Specular.png", nullptr, &(obj->GetSRV()));
       
        assert(obj->GetTRV());
        assert(obj->GetNRV());
        assert(obj->GetSRV());

        D3D11_SAMPLER_DESC sampDesc = {};
        sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
        sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
        sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
        sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
        sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
        sampDesc.MinLOD = 0;
        sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
        m_pDevice->CreateSamplerState(&sampDesc, &(obj->GetSL()));
        assert(obj->GetSL());

    }

    m_objects[0]->GetPos() = { 0,0,0 };
    m_objects[0]->GetScale() = { 1.2,1.2,1.2 };
    m_objects[0]->GetParentObject() = nullptr;

}

void Renderer::UpdateScene()
{
    if (m_camera.nearZ <= 0.0001f) { m_camera.nearZ = 0.0001f; }
    if (m_camera.nearZ >= 9.9f) { m_camera.nearZ = 9.9f; }
    if (m_camera.fovY <= 0.f) { m_camera.fovY = 0.01; }

    m_camera.viewMatrix = DirectX::XMMatrixLookToLH(m_camera.pos, m_camera.dir, m_camera.headDir);
    m_camera.projMatrix = DirectX::XMMatrixPerspectiveFovLH(m_camera.fovY, m_width / (FLOAT)m_height, m_camera.nearZ, m_camera.farZ);

    m_light.EyePosition = m_camera.pos;
}

void Renderer::UpdateObject()
{
    for (auto obj : m_objects)
    {
        Matrix mScale = DirectX::XMMatrixScaling(obj->GetScale().x, obj->GetScale().y, obj->GetScale().z);
        Matrix mRot = DirectX::XMMatrixRotationX(obj->GetRotate().x)
            * DirectX::XMMatrixRotationY(obj->GetRotate().y)
            * DirectX::XMMatrixRotationZ(obj->GetRotate().z);
        Matrix mTrans = DirectX::XMMatrixTranslation(obj->GetPos().x, obj->GetPos().y, obj->GetPos().z);
        Matrix mBasis = DirectX::XMMatrixIdentity();
        if (obj->GetParentObject()) mBasis = obj->GetParentObject()->GetMatrix();
        obj->GetMatrix() = mScale * mRot * mTrans * mBasis;
    }
}

void Renderer::RenderScene()
{
    m_pSwapChain->Present(0, 0);

    m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView, Color{ 0.1f, 0.1f, 0.3f, 1.0f });
    m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

    m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    m_pDeviceContext->IASetInputLayout(m_pInputLayout);
}

void Renderer::RenderImGui()
{
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
    ImGui::SetNextWindowSize(ImVec2(200, 200));

    ImGui::Begin("settings");

    ImGui::Text("camera position");
    ImGui::DragFloat3("##camera", (float*)&(m_camera.pos), 0.1, -100.f, 100.f);

    ImGui::Text("specular power");
    ImGui::SliderFloat("##power", (float*)&(m_material.SpecularPower), 2.0f, 4096.0f);


    ImGui::Text("using");
    ImGui::Checkbox("UseNormalMap", &m_material.UseNormalMap);
    ImGui::Checkbox("UseSpecularMap", &m_light.UseSpecularMap);
    ImGui::End();

    ImGui::Render();
    
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

}

void Renderer::RenderObject()
{
    for (const auto& obj : m_objects)
    {
        m_transform.mWorld = XMMatrixTranspose(obj->GetMatrix());
        m_transform.mView = XMMatrixTranspose(m_camera.viewMatrix);
        m_transform.mProjection = XMMatrixTranspose(m_camera.projMatrix);
        m_pDeviceContext->UpdateSubresource(m_pTransformBuffer, 0, nullptr, &m_transform, 0, 0);

        m_pDeviceContext->UpdateSubresource(m_pLightBuffer, 0, nullptr, &m_light, 0, 0);
        m_pDeviceContext->UpdateSubresource(m_pMaterialBuffer, 0, nullptr, &m_material, 0, 0);

        m_pDeviceContext->IASetVertexBuffers(0, 1, &(obj->GetVB()), &(obj->GetStride()), &(obj->GetOffset()));
        m_pDeviceContext->IASetIndexBuffer(obj->GetIB(), DXGI_FORMAT_R16_UINT, 0);

        m_pDeviceContext->VSSetShader(m_pVertexShader, nullptr, 0);

        m_pDeviceContext->VSSetConstantBuffers(0, 1, &m_pTransformBuffer);
        m_pDeviceContext->VSSetConstantBuffers(1, 1, &m_pLightBuffer);
        m_pDeviceContext->VSSetConstantBuffers(2, 1, &m_pMaterialBuffer);

        m_pDeviceContext->PSSetShader(m_pPixelShader, nullptr, 0);

        m_pDeviceContext->PSSetConstantBuffers(0, 1, &m_pTransformBuffer);
        m_pDeviceContext->PSSetConstantBuffers(1, 1, &m_pLightBuffer);
        m_pDeviceContext->PSSetConstantBuffers(2, 1, &m_pMaterialBuffer);

        m_pDeviceContext->PSSetShaderResources(0, 1, &(obj->GetTRV()));
        m_pDeviceContext->PSSetShaderResources(1, 1, &(obj->GetNRV()));
        m_pDeviceContext->PSSetShaderResources(2, 1, &(obj->GetSRV()));

        m_pDeviceContext->PSSetSamplers(0, 1, &(obj->GetSL()));

        m_pDeviceContext->DrawIndexed(obj->GetIndicies().size(), 0, 0);
    }
}

void Renderer::FinalImGui()
{
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

void Renderer::FianlScene()
{
    Helper::SafeRelease(m_pVertexShader);
    Helper::SafeRelease(m_pPixelShader);
    Helper::SafeRelease(m_pInputLayout);
    Helper::SafeRelease(m_pTransformBuffer);
}

void Renderer::FinalObject()
{
    for (auto& obj : m_objects)
    {
        Helper::SafeRelease(obj->GetIB());
        Helper::SafeRelease(obj->GetVB());
        delete obj;
    }
}

void Renderer::FinalDX()
{
    Helper::SafeRelease(m_pDevice);
    Helper::SafeRelease(m_pDeviceContext);
    Helper::SafeRelease(m_pSwapChain);
    Helper::SafeRelease(m_pRenderTargetView);
    Helper::SafeRelease(m_pDepthStencilView);
}
