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


    for (auto mesh : m_pMeshes)
        mesh->Update();

    for (auto node : m_children)
        node->Update();

}

void Node::Render()
{
    
    for (auto mesh : m_pMeshes)
        mesh->Render();

    for (auto node : m_children)
        node->Render();
}
