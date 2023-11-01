#include "framework.h"
#include "Node.h"

#include "Mesh.h"
#include "Renderer.h"
#include "Model.h"

Renderer* Node::pRenderer = nullptr;

void Node::Update()
{
    Matrix mBasis = DirectX::XMMatrixIdentity();

    if (m_pParent) mBasis = m_pParent->m_worldMatrix;
    else mBasis = m_pOwner->GetMatrix();

    m_worldMatrix = m_relativeMatrix * mBasis;


    for (auto node : m_children)
        node->Update();

    for (auto mesh : m_pMeshes)
        mesh->Update();

}

void Node::Render()
{
    ID3D11DeviceContext* dc = pRenderer->m_pDeviceContext;
    
    pRenderer->m_transform.mWorld = XMMatrixTranspose(m_worldMatrix);
    dc->UpdateSubresource(
        pRenderer->m_pTransformBuffer,
        0,
        nullptr,
        &(pRenderer->m_transform),
        0,
        0);

    dc->VSSetConstantBuffers(0, 1, &(pRenderer->m_pTransformBuffer));
    dc->PSSetConstantBuffers(0, 1, &(pRenderer->m_pTransformBuffer));

    for (auto mesh : m_pMeshes)
        mesh->Render();

    for (auto node : m_children)
        node->Render();
}
