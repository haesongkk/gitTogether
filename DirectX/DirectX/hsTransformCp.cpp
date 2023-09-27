#include "framework.h"
#include "hsTransformCp.h"

void hsTransformCp::Init()
{
}

void hsTransformCp::Update()
{
    Matrix mScale = DirectX::XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z);
    Matrix mRot = DirectX::XMMatrixRotationX(m_rotate.x)
        * DirectX::XMMatrixRotationY(m_rotate.y)
        * DirectX::XMMatrixRotationZ(m_rotate.z);
    Matrix mTrans = DirectX::XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z);
    Matrix mBasis = DirectX::XMMatrixIdentity();

    if (m_parentTransform) mBasis = m_parentTransform->m_matrix;
    m_matrix = mScale * mRot * mTrans * mBasis;
}

void hsTransformCp::Finalize()
{
}