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
		mMeshList[i]->LoadGeomerty();
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
	for (int i = 0; i < m_pASEParser->GetMeshNum(); i++)
	{
		Matrix parent = XMMatrixIdentity();

		for (auto v : mMeshList)
		{
			if (mMeshList[i]->GetMesh()->m_nodeparent.empty() != TRUE
				&& mMeshList[i]->GetMesh()->m_nodeparent == v->GetMesh()->m_nodename)
			{
				parent = v->GetMesh()->m_WorldTM;
				mMeshList[i]->GetMesh()->m_ParentWorldTM = parent;
				break;
			}
		}
		 
		Matrix myLocal = mMeshList[i]->GetMesh()->m_WorldTM * parent.Invert();
		mMeshList[i]->GetMesh()->m_LocalTM = myLocal;
		
		for (int j = 0; j < mMeshList[i]->GetMesh()->m_opt_vertex.size(); j++)
		{
			Vector3 tmpPos = mMeshList[i]->GetMesh()->m_opt_vertex[j]->m_pos;

			mMeshList[i]->GetMesh()->m_opt_vertex[j]->m_pos = XMVector3Transform(tmpPos, mMeshList[i]->GetMesh()->m_WorldTM.Invert());

			tmpPos = mMeshList[i]->GetMesh()->m_opt_vertex[j]->m_pos;

			//mMeshList[i]->GetMesh()->m_opt_vertex[j]->m_pos = XMVector3Transform(tmpPos, parent);
		}
	}

	for (int i = 0; i < m_pASEParser->GetMeshNum(); i++)
	{

		for (auto v : mMeshList)
		{
			if (mMeshList[i]->GetMesh()->m_nodeparent.empty() != TRUE
				&& mMeshList[i]->GetMesh()->m_nodeparent == v->GetMesh()->m_nodename)
				mMeshList[i]->GetMesh()->m_ParentLocalTM = v->GetMesh()->m_LocalTM;


		}

		for (int j = 0; j < mMeshList[i]->GetMesh()->m_opt_vertex.size(); j++)
		{
			Vector3 tmpPos = mMeshList[i]->GetMesh()->m_opt_vertex[j]->m_pos;

			mMeshList[i]->GetMesh()->m_opt_vertex[j]->m_pos = XMVector3Transform(tmpPos, parent);
		}


	}


}

void Model::FindChild()
{
	for (int i = 0; i < m_pASEParser->GetMeshNum(); i++)
	{
		if (mMeshList[i]->GetMesh()->m_nodeparent.empty() == FALSE)
		{

		}
	}
}
