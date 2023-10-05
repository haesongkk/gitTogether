#include "framework.h"
#include "hsRenderCp.h"
#include "Helper.h"

void hsRenderCp::Init()
{
    vector<Vertex> verticies =
    {
        Vertex( Vector3(-1.0f, 1.0f,-1.0f), Vector3(0.0f, 1.0f, 0.0f), Vector2(1.0f, 0.0f) ), 
        Vertex( Vector3(1.0f, 1.0f,-1.0f),  Vector3(0.0f, 1.0f, 0.0f), Vector2(0.0f, 0.0f) ),
        Vertex( Vector3(1.0f, 1.0f, 1.0f),  Vector3(0.0f, 1.0f, 0.0f), Vector2(0.0f, 1.0f) ),
        Vertex( Vector3(-1.0f, 1.0f, 1.0f), Vector3(0.0f, 1.0f, 0.0f), Vector2(1.0f, 1.0f) ),
        
        Vertex( Vector3(-1.0f,-1.0f,-1.0f), Vector3(0.0f,-1.0f, 0.0f), Vector2(0.0f, 0.0f) ),  // 아랫면이라 y전부 -1
        Vertex( Vector3(1.0f,-1.0f,-1.0f),  Vector3(0.0f,-1.0f, 0.0f), Vector2(1.0f, 0.0f) ),
        Vertex( Vector3(1.0f,-1.0f, 1.0f),  Vector3(0.0f,-1.0f, 0.0f), Vector2(1.0f, 1.0f) ),
        Vertex( Vector3(-1.0f,-1.0f, 1.0f), Vector3(0.0f,-1.0f, 0.0f), Vector2(0.0f, 1.0f) ),

        Vertex( Vector3(-1.0f,-1.0f, 1.0f), Vector3(-1.0f, 0.0f, 0.0f), Vector2(0.0f, 1.0f) ),	// 왼쪽면 이라 x전부 -1
        Vertex( Vector3(-1.0f,-1.0f,-1.0f), Vector3(-1.0f, 0.0f, 0.0f), Vector2(1.0f, 1.0f) ),
        Vertex( Vector3(-1.0f, 1.0f,-1.0f), Vector3(-1.0f, 0.0f, 0.0f), Vector2(1.0f, 0.0f) ),
        Vertex( Vector3(-1.0f, 1.0f, 1.0f), Vector3(-1.0f, 0.0f, 0.0f), Vector2(0.0f, 0.0f) ),

        Vertex( Vector3(1.0f,-1.0f, 1.0f),  Vector3(1.0f, 0.0f, 0.0f), Vector2(1.0f, 1.0f) ),	// 오른쪽면 이라 x전부 +1
        Vertex( Vector3(1.0f,-1.0f,-1.0f),  Vector3(1.0f, 0.0f, 0.0f), Vector2(0.0f, 1.0f) ),
        Vertex( Vector3(1.0f, 1.0f,-1.0f),  Vector3(1.0f, 0.0f, 0.0f), Vector2(0.0f, 0.0f) ),
        Vertex( Vector3(1.0f, 1.0f, 1.0f),  Vector3(1.0f, 0.0f, 0.0f), Vector2(1.0f, 0.0f) ),

        Vertex( Vector3(-1.0f,-1.0f,-1.0f), Vector3(0.0f, 0.0f,-1.0f), Vector2(0.0f, 1.0f) ),  // 앞면이라 z전부 -1
        Vertex( Vector3(1.0f,-1.0f,-1.0f),  Vector3(0.0f, 0.0f,-1.0f), Vector2(1.0f, 1.0f) ),
        Vertex( Vector3(1.0f, 1.0f,-1.0f),  Vector3(0.0f, 0.0f,-1.0f), Vector2(1.0f, 0.0f) ),
        Vertex( Vector3(-1.0f, 1.0f,-1.0f), Vector3(0.0f, 0.0f,-1.0f), Vector2(0.0f, 0.0f) ),

        Vertex( Vector3(-1.0f,-1.0f, 1.0f), Vector3(0.0f, 0.0f, 1.0f), Vector2(1.0f, 1.0f) ),	//뒷면이라 z전부 +1
        Vertex( Vector3(1.0f,-1.0f, 1.0f),  Vector3(0.0f, 0.0f, 1.0f), Vector2(0.0f, 1.0f) ),
        Vertex( Vector3(1.0f, 1.0f, 1.0f),  Vector3(0.0f, 0.0f, 1.0f), Vector2(0.0f, 0.0f) ),
        Vertex( Vector3(-1.0f, 1.0f, 1.0f), Vector3(0.0f, 0.0f, 1.0f), Vector2(1.0f, 0.0f) ),
    };
    vector<UINT> indicies =
    {
        3,1,0,    2,1,3,
        6,4,5,    7,4,6,
        11,9,8,   10,9,11,
        14,12,13, 15,12,14,
        19,17,16, 18,17,19,
        22,20,21, 23,20,22
    };
    CreateVertexShader(L"./Shaders/BasicVertexShader.hlsl");
    CreatePixelShader(L"./Shaders/BasicPixelShader.hlsl");
    CreateTexture(L"./Texture/seafloor.dds");
    CreateVertexBuffer(verticies);
    CreateIndexBuffer(indicies);
    CreateContantBuffer();
}

void hsRenderCp::Update()
{
    m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    m_pDeviceContext->IASetInputLayout(m_pInputLayout);

    m_pDeviceContext->VSSetShader(m_pVertexShader, nullptr, 0);
    m_pDeviceContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);

    m_pDeviceContext->PSSetShader(m_pPixelShader, nullptr, 0);
    m_pDeviceContext->PSSetConstantBuffers(0, 1, &m_pConstantBuffer);

    m_pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &m_VertextBufferStride, &m_VertextBufferOffset);
    m_pDeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);

    m_pDeviceContext->PSSetSamplers(0, 1, &m_pSamplerLinear);
    m_pDeviceContext->PSSetShaderResources(0, 1, &m_pTextureRV);

    //cb.mWorld = DirectX::XMMatrixTranspose(obj->GetMatrix());
    m_pDeviceContext->UpdateSubresource(m_pConstantBuffer, 0, nullptr, m_pCB, 0, 0);

    m_pDeviceContext->DrawIndexed(m_indexCount, 0, 0);
}

void hsRenderCp::Finalize()
{
    Helper::SafeRelease(m_pIndexBuffer);
    Helper::SafeRelease(m_pVertexBuffer);

    Helper::SafeRelease(m_pVertexShader);
    Helper::SafeRelease(m_pPixelShader);
    Helper::SafeRelease(m_pInputLayout);
    Helper::SafeRelease(m_pConstantBuffer);
}

void hsRenderCp::CreateVertexShader(const wstring& _filePath)
{
    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    ID3DBlob* vertexShaderBuffer = nullptr;

    D3DCompileFromFile(
        _filePath.c_str(),
        nullptr, 
        D3D_COMPILE_STANDARD_FILE_INCLUDE, 
        "main", 
        "vs_4_0",
        0, 
        0,
        &vertexShaderBuffer, 
        nullptr);
    assert(vertexShaderBuffer);

    m_pDevice->CreateVertexShader(
        vertexShaderBuffer->GetBufferPointer(),
        vertexShaderBuffer->GetBufferSize(), 
        NULL,
        &m_pVertexShader);
    assert(m_pVertexShader);

    m_pDevice->CreateInputLayout(
        layout, 
        ARRAYSIZE(layout),
        vertexShaderBuffer->GetBufferPointer(), 
        vertexShaderBuffer->GetBufferSize(), 
        &m_pInputLayout);
    assert(m_pInputLayout);

    Helper::SafeRelease(vertexShaderBuffer);
}

void hsRenderCp::CreatePixelShader(const wstring& _filePath)
{
    ID3DBlob* pixelShaderBuffer = nullptr;

    D3DCompileFromFile(
        _filePath.c_str(),
        nullptr, 
        D3D_COMPILE_STANDARD_FILE_INCLUDE, 
        "main",
        "ps_4_0",
        0,
        0, 
        &pixelShaderBuffer, 
        nullptr);
    assert(pixelShaderBuffer);

    m_pDevice->CreatePixelShader(
        pixelShaderBuffer->GetBufferPointer(),
        pixelShaderBuffer->GetBufferSize(),
        NULL, 
        &m_pPixelShader);
    assert(m_pPixelShader);

    Helper::SafeRelease(pixelShaderBuffer);
}

void hsRenderCp::CreateVertexBuffer(vector<Vertex>& _vertices, UINT _offset)
{
    D3D11_BUFFER_DESC vbDesc = {};
    auto a = sizeof(Vertex);
    vbDesc.ByteWidth = sizeof(Vertex) * _vertices.size();
    vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vbDesc.Usage = D3D11_USAGE_DEFAULT;

    D3D11_SUBRESOURCE_DATA vbData = {};
    vbData.pSysMem = _vertices.data();

    m_pDevice->CreateBuffer(&vbDesc, &vbData, &m_pVertexBuffer);

    assert(m_pVertexBuffer);

    m_VertextBufferStride = sizeof(Vertex);
    m_VertextBufferOffset = _offset;
}

void hsRenderCp::CreateIndexBuffer(vector<UINT>& _indicies)
{
    D3D11_BUFFER_DESC ibDesc = {};
    ibDesc.ByteWidth = sizeof(WORD) * m_indexCount;
    ibDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    ibDesc.Usage = D3D11_USAGE_DEFAULT;
    ibDesc.CPUAccessFlags = 0;

    D3D11_SUBRESOURCE_DATA ibData = {};
    ibData.pSysMem = _indicies.data();

    m_pDevice->CreateBuffer(&ibDesc, &ibData, &m_pIndexBuffer);

    assert(m_pIndexBuffer);

    m_indexCount = _indicies.size();
}

void hsRenderCp::CreateTexture(const wstring& _filePath)
{
    D3D11_SAMPLER_DESC sampDesc = {};
    sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    sampDesc.MinLOD = 0;
    sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
    m_pDevice->CreateSamplerState(&sampDesc, &m_pSamplerLinear);
    assert(m_pSamplerLinear);

    CreateDDSTextureFromFile(m_pDevice, _filePath.c_str(), nullptr, &m_pTextureRV);
    assert(m_pTextureRV);
}

void hsRenderCp::CreateContantBuffer()
{
    m_pCB = new ConstantBuffer;

    D3D11_BUFFER_DESC cbDesc = {};
    cbDesc.Usage = D3D11_USAGE_DEFAULT;
    cbDesc.ByteWidth = sizeof(ConstantBuffer);
    cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    cbDesc.CPUAccessFlags = 0;
    m_pDevice->CreateBuffer(&cbDesc, nullptr, &m_pConstantBuffer);
    assert(m_pConstantBuffer);
}