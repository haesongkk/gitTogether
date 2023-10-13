#include "framework.h"
#include "Mesh.h"

#include "Renderer.h"
#include "Helper.h"
Renderer* Mesh::pRenderer = nullptr;

void Mesh::Init()
{
    vector<Vertex> vertices =
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

    vector<WORD> indices =
    {
        3,1,0,    2,1,3,
        6,4,5,    7,4,6,
        11,9,8,   10,9,11,
        14,12,13, 15,12,14,
        19,17,16, 18,17,19,
        22,20,21, 23,20,22
    };

    CreateVertexBuffer(vertices);
    CreateIndexBuffer(indices);
    CreateResourceView(L"./Texture/Bricks059_1K-JPG_Color.jpg", &pTRV);
    CreateResourceView(L"./Texture/Bricks059_1K-JPG_NormalDX.jpg", &pNRV);
    CreateResourceView(L"./Texture/Bricks059_Specular.png", &pSRV);
}

void Mesh::Update()
{
    Matrix mScale = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
    Matrix mRot = DirectX::XMMatrixRotationX(rotate.x)
        * DirectX::XMMatrixRotationY(rotate.y)
        * DirectX::XMMatrixRotationZ(rotate.z);
    Matrix mTrans = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
    Matrix mBasis = DirectX::XMMatrixIdentity();
    if (pParentObject) mBasis = pParentObject->matrix;
    matrix = mScale * mRot * mTrans * mBasis;
}

void Mesh::Render()
{
    ID3D11DeviceContext* dc = pRenderer->m_pDeviceContext;

    pRenderer->m_transform.mWorld = XMMatrixTranspose(matrix);
    dc->UpdateSubresource(
        pRenderer->m_pTransformBuffer,
        0,
        nullptr,
        &(pRenderer->m_transform),
        0,
        0);

    dc->VSSetConstantBuffers(0, 1, &(pRenderer->m_pTransformBuffer));
    dc->PSSetConstantBuffers(0, 1, &(pRenderer->m_pTransformBuffer));

    dc->IASetVertexBuffers(0, 1, &pVB, &VertextBufferStride, &VertextBufferOffset);
    dc->IASetIndexBuffer(pIB, DXGI_FORMAT_R16_UINT, 0);

    dc->PSSetShaderResources(0, 1, &pTRV);
    dc->PSSetShaderResources(1, 1, &pNRV);
    dc->PSSetShaderResources(2, 1, &pSRV);
    dc->PSSetSamplers(0, 1, &(pRenderer->m_pSamplerLinear));

    dc->DrawIndexed(indexCount, 0, 0);
}

void Mesh::Final()
{
    Helper::SafeRelease(pIB);
    Helper::SafeRelease(pVB);
    Helper::SafeRelease(pTRV);
    Helper::SafeRelease(pNRV);
    Helper::SafeRelease(pSRV);
    delete this;
}

void Mesh::CreateVertexBuffer(vector<Vertex>& _vertices)
{
    VertextBufferStride = sizeof(Vertex);
    VertextBufferOffset = 0;

    D3D11_BUFFER_DESC vbDesc = {};
    auto a = sizeof(Vertex);
    vbDesc.ByteWidth = sizeof(Vertex) * _vertices.size();
    vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vbDesc.Usage = D3D11_USAGE_DEFAULT;

    D3D11_SUBRESOURCE_DATA vbData = {};
    vbData.pSysMem = _vertices.data();
    pRenderer->m_pDevice->CreateBuffer(&vbDesc, &vbData, &pVB);

    assert(pVB);
}

void Mesh::CreateIndexBuffer(vector<WORD>& _indices)
{
    indexCount = _indices.size();

    D3D11_BUFFER_DESC ibDesc = {};
    ibDesc.ByteWidth = sizeof(WORD) * indexCount;
    ibDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    ibDesc.Usage = D3D11_USAGE_DEFAULT;
    ibDesc.CPUAccessFlags = 0;

    D3D11_SUBRESOURCE_DATA ibData = {};
    ibData.pSysMem = _indices.data();

    pRenderer->m_pDevice->CreateBuffer(&ibDesc, &ibData, &pIB);
    assert(pIB);
}

void Mesh::CreateResourceView(const wstring& _filePath, ID3D11ShaderResourceView** _ppRV)
{
    HRESULT hr;
    hr = CreateDDSTextureFromFile(pRenderer->m_pDevice, _filePath.c_str(), nullptr, _ppRV);
    if (FAILED(hr))
        CreateWICTextureFromFile(pRenderer->m_pDevice, _filePath.c_str(), nullptr, _ppRV);
    assert(*_ppRV);
}