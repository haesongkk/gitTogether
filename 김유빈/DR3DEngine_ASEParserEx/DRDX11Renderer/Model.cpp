#include "Model.h"
#include "MeshObject.h"

#include "DRCamera.h"
#include "DDSTextureLoader.h"
#include "GeometryGenerator.h"
#include "../DR3DLib/CASEParser.h"


Model::Model(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, ID3D11RasterizerState* pRS)
	: md3dDevice(pDevice), md3dImmediateContext(pDeviceContext), m_pRenderstate(pRS),
	mVB(nullptr), mIB(nullptr),
	mEyePosW(0.0f, 0.0f, 0.0f)
{
	m_pASEParser = new CASEParser();
	m_pASEParser->Init();
	m_pASEParser->Load((LPSTR)"../ASEFile/03IK-Joe_onlymesh.ASE");
}

Model::~Model()
{
}

void Model::Initialize()
{
	for (int i = 0; i < m_pASEParser->GetMeshNum(); i++)
	{
		// 파서 변환부
		m_pASEParser->ConvertAll(m_pASEParser->GetMesh(i));

		// 메시 관련 초기화 
		MeshObject* mesh = new MeshObject(md3dDevice, md3dImmediateContext, m_pRenderstate);
		mesh->Initialize(m_pASEParser->GetMesh(i));
		mMeshList.push_back(mesh);
	}

	SetHierarchy();

	for (int i = 0; i < m_pASEParser->GetMeshNum(); i++)
	{
		// 메시 관련 정점, 인덱스 버퍼 설정
		mMeshList[i]->LoadGeomerty(mMeshList[i]->GetMesh());
	}
}

void Model::Update(DRCamera* pCamera)
{
	for (int i = 0; i < m_pASEParser->GetMeshNum(); i++)
	{
		mMeshList[i]->Update(pCamera);
	}
}

void Model::Render()
{
	for (int i = 0; i < m_pASEParser->GetMeshNum(); i++)
	{
		mMeshList[i]->Render();
	}
}

void Model::SetHierarchy()
{
	std::vector<XMFLOAT4X4*> myNodeTM;

	for (int i = 0; i < m_pASEParser->GetMeshNum(); i++)
	{
		XMFLOAT4X4* node = new XMFLOAT4X4;

		node->_11 = mMeshList[i]->GetMesh()->m_tm_row0.x;
		node->_12 = mMeshList[i]->GetMesh()->m_tm_row0.y;
		node->_13 = mMeshList[i]->GetMesh()->m_tm_row0.z;
		node->_21 = mMeshList[i]->GetMesh()->m_tm_row1.x;
		node->_22 = mMeshList[i]->GetMesh()->m_tm_row1.y;
		node->_23 = mMeshList[i]->GetMesh()->m_tm_row1.z;
		node->_31 = mMeshList[i]->GetMesh()->m_tm_row2.x;
		node->_32 = mMeshList[i]->GetMesh()->m_tm_row2.y;
		node->_33 = mMeshList[i]->GetMesh()->m_tm_row2.z;
		node->_41 = mMeshList[i]->GetMesh()->m_tm_row3.x;
		node->_42 = mMeshList[i]->GetMesh()->m_tm_row3.y;
		node->_43 = mMeshList[i]->GetMesh()->m_tm_row3.z;

		myNodeTM.push_back(node);
	}

	XMFLOAT4X4 parentNodeTM;
	XMMATRIX parent;

	for (int i = 0; i < m_pASEParser->GetMeshNum(); i++)
	{
		for (auto v : mMeshList)
		{
			if (!mMeshList[i]->GetMesh()->m_nodeparent.empty()
				&& mMeshList[i]->GetMesh()->m_nodeparent == v->GetMesh()->m_nodename)
			{
				XMFLOAT4X4 vNodeTM;

				vNodeTM._11 = v->GetMesh()->m_tm_row0.x;
				vNodeTM._12 = v->GetMesh()->m_tm_row0.y;
				vNodeTM._13 = v->GetMesh()->m_tm_row0.z;
				vNodeTM._21 = v->GetMesh()->m_tm_row1.x;
				vNodeTM._22 = v->GetMesh()->m_tm_row1.y;
				vNodeTM._23 = v->GetMesh()->m_tm_row1.z;
				vNodeTM._31 = v->GetMesh()->m_tm_row2.x;
				vNodeTM._32 = v->GetMesh()->m_tm_row2.y;
				vNodeTM._33 = v->GetMesh()->m_tm_row2.z;
				vNodeTM._41 = v->GetMesh()->m_tm_row3.x;
				vNodeTM._42 = v->GetMesh()->m_tm_row3.y;
				vNodeTM._43 = v->GetMesh()->m_tm_row3.z;

				parentNodeTM = vNodeTM;
				parent = XMLoadFloat4x4(&parentNodeTM);
			}
			else
			{
				//parent = MathHelper::InverseTranspose(XMMatrixIdentity());
			}
		}

		XMMATRIX myLocal = XMLoadFloat4x4(myNodeTM[i]) * MathHelper::InverseTranspose(parent);

		XMStoreFloat4x4(myNodeTM[i], myLocal);

		for (int j = 0; j < mMeshList[i]->GetMesh()->m_opt_vertex.size(); j++)
		{
			Vector3 tmp = mMeshList[i]->GetMesh()->m_opt_vertex[j]->m_pos;
			mMeshList[i]->GetMesh()->m_opt_vertex[j]->m_pos = XMVector3TransformNormal(XMLoadFloat3(&tmp), myLocal);
		}
	}
}
