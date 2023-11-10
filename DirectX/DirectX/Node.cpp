#include "framework.h"
#include "Node.h"

#include "Mesh.h"
#include "Renderer.h"
#include "Model.h"
#include "Transform.h"

Node::Node(string name, shared_ptr<Model> spOwnerModel , Matrix localTM, vector<int> connectedMeshIndices, vector<string> childrenNodeNames)
    :m_name{ name }
    , m_wpOwnerModel{ spOwnerModel }
    , m_worldMatrix { XMMatrixIdentity() }
    , m_relativeMatrix{ localTM }
    , m_connectedMeshIndicies { connectedMeshIndices }
    , m_childrenNodeNames { childrenNodeNames }
{
}

void Node::Run()
{
    Matrix mBasis = DirectX::XMMatrixIdentity();

    if (auto node = m_wpParentNode.lock())
        mBasis = node->m_worldMatrix;
    else if (auto gameObject = m_wpOwnerModel.lock())
        mBasis = gameObject->GetTransform()->GetWorldTM();

    m_worldMatrix = m_relativeMatrix * mBasis;


    ID3D11DeviceContext* dc = m_wpOwnerModel.lock()->m_wpRenderer.lock()->m_pDeviceContext;

    dc->UpdateSubresource(m_pWorldTMBuffer, 0, nullptr, &m_worldTM, 0, 0);
    dc->VSSetConstantBuffers(0, 1, &m_pWorldTMBuffer);
    dc->PSSetConstantBuffers(0, 1, &m_pWorldTMBuffer);

    for (auto meshIndex : m_connectedMeshIndicies)
        m_wpOwnerModel.lock()->GetMesh(meshIndex)->Run();

    for (auto nodeName : m_childrenNodeNames)
        m_wpOwnerModel.lock()->GetNode(nodeName)->Run();
}