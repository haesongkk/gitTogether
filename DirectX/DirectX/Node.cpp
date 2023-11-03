#include "framework.h"
#include "Node.h"

#include "Mesh.h"
#include "Renderer.h"
#include "Model.h"

Renderer* Node::pRenderer = nullptr;

void Node::Render()
{
    Matrix mBasis = DirectX::XMMatrixIdentity();

    if (m_pParent) mBasis = m_pParent->m_worldMatrix;
    else mBasis = m_pOwner->GetMatrix();

    m_worldMatrix = m_relativeMatrix * mBasis;

    for (auto node : m_children)
        node->Render();
}