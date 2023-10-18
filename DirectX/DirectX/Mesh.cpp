#include "framework.h"
#include "Mesh.h"

#include "Renderer.h"
#include "Helper.h"
#include "GameObject.h"

Renderer* Mesh::pRenderer = nullptr;
Renderer* Material::pRenderer = nullptr;

Mesh::Mesh(GameObject* _pOwner)
    :m_pOwner(_pOwner)
{
}

void Mesh::Init()
{
}

void Mesh::Render()
{
    ID3D11DeviceContext* dc = pRenderer->m_pDeviceContext;

    pRenderer->m_transform.mWorld = XMMatrixTranspose(m_pOwner->GetMatrix());
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

    
    dc->PSSetSamplers(0, 1, &(pRenderer->m_pSamplerLinear));

    dc->DrawIndexed(indexCount, 0, 0);
}

void Mesh::Final()
{
    Helper::SafeRelease(pIB);
    Helper::SafeRelease(pVB);
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

Material::Material(GameObject* _pOwner)
    :m_pOwner(_pOwner)
{
}

void Material::Init()
{
}

void Material::Render()
{
    ID3D11DeviceContext* dc = pRenderer->m_pDeviceContext;

    for (int i = 0; i < TextureIndex::End; i++)
        dc->PSSetShaderResources(0, i, &(m_pTextures[i]));
}

void Material::Final()
{
    for (int i = 0; i < TextureIndex::End; i++)
        Helper::SafeRelease(m_pTextures[i]);
}

void Material::CreateTextureFromFile(const wstring& _filePath, TextureIndex _txId)
{
    CreateDDSTextureFromFile(pRenderer->m_pDevice, _filePath.c_str(), nullptr, &(m_pTextures[_txId]));

    if(!m_pTextures[_txId])
        CreateWICTextureFromFile(pRenderer->m_pDevice, _filePath.c_str(), nullptr, &(m_pTextures[_txId]));

    assert(m_pTextures[_txId]);
}