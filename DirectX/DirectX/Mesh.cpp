#include "framework.h"
#include "Mesh.h"

#include "Renderer.h"
#include "Helper.h"
#include "Model.h"
#include "Material.h"
#include "Bone.h"
#include "Node.h"

Renderer* Mesh::pRenderer = nullptr;

Mesh::Mesh(Model* _pOwner)
    :m_pOwner(_pOwner)
{
}

Mesh::~Mesh()
{
    Helper::SafeRelease(pIB);
    Helper::SafeRelease(pVB);
}


void Mesh::CreateVertexBuffer(vector<Vertex>& _vertices)
{
    VertextBufferStride = sizeof(Vertex);
    VertextBufferOffset = 0;

    D3D11_BUFFER_DESC vbDesc = {};
    auto a = sizeof(Vertex);
    vbDesc.ByteWidth = sizeof(Vertex) * _vertices.size();
    vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vbDesc.Usage = D3D11_USAGE_DEFAULT;

    D3D11_SUBRESOURCE_DATA vbData = {};
    vbData.pSysMem = _vertices.data();
    pRenderer->m_pDevice->CreateBuffer(&vbDesc, &vbData, &pVB);

    assert(pVB);
}

void Mesh::CreateIndexBuffer(vector<WORD>& _indices)
{
    indexCount = _indices.size();

    D3D11_BUFFER_DESC ibDesc = {};
    ibDesc.ByteWidth = sizeof(WORD) * indexCount;
    ibDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    ibDesc.Usage = D3D11_USAGE_DEFAULT;
    ibDesc.CPUAccessFlags = 0;

    D3D11_SUBRESOURCE_DATA ibData = {};
    ibData.pSysMem = _indices.data();

    pRenderer->m_pDevice->CreateBuffer(&ibDesc, &ibData, &pIB);
    assert(pIB);
}

void Mesh::SetMaterialIndex(UINT _index)
{
    m_pConnectMaterial = m_pOwner->m_pMaterials[_index];
}

bool Vertex::AddBoneData(int _index, float _weight)
{
    for (int i = 0; i < 4; i++)
    {
        if (boneWeights[i] == 0.f)
        {
            boneIndices[i] = _index;
            boneWeights[i] = _weight;
            return true;
        }
    }
    return false; // 정보 저장 실패
}

