#include "framework.h"
#include "GameObject.h"
#include "Mesh.h"
#include "Material.h"
#include "Renderer.h"
#include "Node.h"
#include "Animation.h"

void GameObject::Update()
{
    Matrix mScale = DirectX::XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z);
    Matrix mRot = DirectX::XMMatrixRotationX(m_rotate.x)
        * DirectX::XMMatrixRotationY(m_rotate.y)
        * DirectX::XMMatrixRotationZ(m_rotate.z);
    Matrix mTrans = DirectX::XMMatrixTranslation(m_position.x, m_position.y, m_position.z);

    Matrix mBasis = DirectX::XMMatrixIdentity();
    if (m_pParentObject) mBasis = m_pParentObject->GetMatrix();
    m_matrix = mScale * mRot * mTrans * mBasis;


    for (auto anim : m_pAnimations)
        anim->Update();

    m_pRootNode->Update();
}

void GameObject::Render()
{
    m_pRootNode->Render();
}
