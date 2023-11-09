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
	m_pASEParser->Load((LPSTR)"../ASEFile/03IK-Joe.ASE");
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

void Model::Update(DRCamera* pCamera, float _deltaTime)
{
	for (int i = 0; i < m_pASEParser->GetMeshNum(); i++)
	{
		mMeshList[i]->Update(pCamera, _deltaTime);
	}

	//XMMATRIX mRot, mLocal;

	//int index = 57;

	//for (int i = 0; i < m_pASEParser->GetMeshNum(); i++)
	//{
	//	if (mMeshList[i]->GetMesh()->m_nodename == "Bone01")
	//	{
	//		Mesh* m = mMeshList[i]->GetMesh();
	//		static float angle = 0;

	//		angle += 0.00001;
	//		mLocal = XMMatrixRotationX(angle) * XMLoadFloat4x4(&(m->m_LocalTM));
	//		XMStoreFloat4x4(&(m->m_LocalTM), mLocal);
	//	}
	//}


	for (int i = 0; i < m_pASEParser->GetMeshNum(); i++)
	{
		mMeshList[i]->UpdateAnimation(_deltaTime);
	}

	SetHierarchy();
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
	// index의 로컬이 변하면 걔의 자식들도 월드tm 이 변해야하니까
	// 전체적으로 다시 월드 tm 구해주기
	for (int i = 0; i < m_pASEParser->GetMeshNum(); i++)
	{
		mMeshList[i]->GetMesh()->m_WorldTM = WorldTM(mMeshList[i]);
	}
}

/// <summary>
/// 로컬을 월드로 변환, 월드를 반환
/// </summary>
/// <param name="mesh"></param>
/// <returns></returns>
Matrix Model::WorldTM(MeshObject* mesh)
{
	//if (mesh->GetMesh()->m_nodeparent.empty())
	//	mesh->GetMesh()->m_WorldTM = mesh->GetMesh()->m_LocalTM;
	//else
	{
		for (auto& v : mMeshList)
		{
			if (mesh->GetMesh()->m_nodeparent == v->GetMesh()->m_nodename)
				return mesh->GetMesh()->m_LocalTM * WorldTM(v);
		}
	}

	return mesh->GetMesh()->m_WorldTM;
}

