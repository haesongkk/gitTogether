#include "framework.h"
#include "GameObject.h"
#include "Mesh.h"
#include "Material.h"

void GameObject::Init()
{
    m_pMeshes.push_back(new Mesh(this));
    for (auto obj : m_pMaterials) obj->Init();
    for (auto obj : m_pMeshes) obj->Init();
}

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
}

void GameObject::Render()
{
    //for (auto obj : m_pMaterials) obj->Render();
    for (auto obj : m_pMeshes) obj->Render();
}

void GameObject::Final()
{
    for (auto obj : m_pMaterials) obj->Final();
    for (auto obj : m_pMeshes) obj->Final();
}
