#include "framework.h"
#include "Node.h"

#include "Mesh.h"
#include "Renderer.h"
#include "GameObject.h"

Renderer* Node::pRenderer = nullptr;

void Node::Update()
{
    ID3D11DeviceContext* dc = pRenderer->m_pDeviceContext;

    Matrix mBasis = DirectX::XMMatrixIdentity();

    if (m_pParent) mBasis = m_pParent->m_matrix;
    else mBasis = m_pOwner->GetMatrix();

    Matrix matrix =  m_matrix * mBasis;
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

    for (auto mesh : m_pMeshes)
        mesh->Render();

    for (auto node : m_children)
        node->Update();
}
