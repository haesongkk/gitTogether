#include "framework.h"
#include "Model.h"
#include "Mesh.h"
#include "Material.h"
#include "Renderer.h"
#include "Node.h"
#include "Animation.h"
#include "Bone.h"

void Model::Render()
{
    UpdateModelTM();

    for (auto anim : m_pAnimations)
        anim->Update();

    m_pRootNode->Render();

    for (auto mesh : m_pMeshes)
        mesh->Render();

}

void Model::UpdateModelTM()
{
    Matrix mScale = Matrix::CreateScale(m_scale);
    Matrix mRot = Matrix::CreateRotationX(m_rotate.x)
        * Matrix::CreateRotationY(m_rotate.y)
        * Matrix::CreateRotationZ(m_rotate.z);

    Matrix mTrans = Matrix::CreateTranslation(m_position);

    Matrix mBasis = DirectX::XMMatrixIdentity();
    if (m_pParentObject) mBasis = m_pParentObject->GetMatrix();

    m_matrix = mScale * mRot * mTrans * mBasis;
}

