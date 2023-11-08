#include "framework.h"
#include "Mesh.h"

#include "Renderer.h"
#include "Helper.h"
#include "Model.h"
#include "Material.h"
#include "Bone.h"
#include "Node.h"
#include "Vertex.h"

Renderer* Mesh::pRenderer = nullptr;

Mesh::Mesh(Model* _pOwner)
    :m_pOwner(_pOwner)
{
}

Mesh::~Mesh()
{
    Helper::SafeRelease(pIB);
    Helper::SafeRelease(pVB);
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

void Mesh::SetMaterialIndex(UINT _index)
{
    m_pConnectMaterial = m_pOwner->m_pMaterials[_index];
}

void Mesh::Render()
{
    ID3D11DeviceContext* dc = pRenderer->m_pDeviceContext;

    dc->IASetVertexBuffers(0, 1, &pVB, &VertextBufferStride, &VertextBufferOffset);
    dc->IASetIndexBuffer(pIB, DXGI_FORMAT_R16_UINT, 0);

    m_pConnectMaterial->Render();

    assert(m_pBones.size() < 128);
    for (auto bone : m_pBones) bone->Update();

    dc->UpdateSubresource(pRenderer->m_pBonesBuffer, 0, nullptr, &(pRenderer->m_bones), 0, 0);
    dc->VSSetConstantBuffers(4, 1, &(pRenderer->m_pBonesBuffer));
    dc->PSSetConstantBuffers(4, 1, &(pRenderer->m_pBonesBuffer));

    pRenderer->m_transform.mWorld = m_pOwner->GetMatrix();
    dc->UpdateSubresource(pRenderer->m_pTransformBuffer, 0, nullptr, &(pRenderer->m_transform), 0, 0);
    dc->VSSetConstantBuffers(0, 1, &(pRenderer->m_pTransformBuffer));
    dc->PSSetConstantBuffers(0, 1, &(pRenderer->m_pTransformBuffer));

    dc->DrawIndexed(indexCount, 0, 0);
}