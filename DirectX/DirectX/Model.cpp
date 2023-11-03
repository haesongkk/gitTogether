#include "framework.h"
#include "Model.h"
#include "Mesh.h"
#include "Material.h"
#include "Renderer.h"
#include "Node.h"
#include "Animation.h"
#include "Bone.h"

Renderer* Model::pRenderer = nullptr;

void Model::Render()
{
    UpdateModelTM();
    UpdateNodeTM();

    ID3D11DeviceContext* dc = pRenderer->m_pDeviceContext;

    for (auto mesh : m_pMeshes)
    {
        dc->IASetVertexBuffers(0, 1, &mesh->pVB, &mesh->VertextBufferStride, &mesh->VertextBufferOffset);
        dc->IASetIndexBuffer(mesh->pIB, DXGI_FORMAT_R16_UINT, 0);

        mesh->m_pConnectMaterial->Render();

        assert(mesh->m_pBones.size() < 128);
        for (auto bone : mesh->m_pBones) bone->Update();

        dc->UpdateSubresource(pRenderer->m_pBonesBuffer, 0, nullptr, &(pRenderer->m_bones), 0, 0);
        dc->VSSetConstantBuffers(4, 1, &(pRenderer->m_pBonesBuffer));
        dc->PSSetConstantBuffers(4, 1, &(pRenderer->m_pBonesBuffer));

        pRenderer->m_transform.mWorld = GetMatrix();
        dc->UpdateSubresource(pRenderer->m_pTransformBuffer, 0, nullptr, &(pRenderer->m_transform), 0, 0);
        dc->VSSetConstantBuffers(0, 1, &(pRenderer->m_pTransformBuffer));
        dc->PSSetConstantBuffers(0, 1, &(pRenderer->m_pTransformBuffer));

        dc->DrawIndexed(mesh->indexCount, 0, 0);
    }

}

void Model::UpdateModelTM()
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

void Model::UpdateNodeTM()
{
    for (auto anim : m_pAnimations)
        anim->Update();
    m_pRootNode->Render();
}
