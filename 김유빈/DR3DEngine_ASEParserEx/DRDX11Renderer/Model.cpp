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
		// �ļ� ��ȯ��
		m_pASEParser->ConvertAll(m_pASEParser->GetMesh(i));

		// �޽� ���� �ʱ�ȭ 
		MeshObject* mesh = new MeshObject(md3dDevice, md3dImmediateContext, m_pRenderstate);
		mesh->Initialize(m_pASEParser->GetMesh(i));
		m_NodeMesh.insert(pair<string, MeshObject*>(mesh->GetMesh()->m_nodename, mesh));
		mMeshList.push_back(mesh);
	}

	SetHierarchy();

	for (int i = 0; i < m_pASEParser->GetMeshNum(); i++)
	{
		// �޽� ���� ����, �ε��� ���� ����
		mMeshList[i]->LoadGeomerty();
	}
}

void Model::Update(DRCamera* pCamera, float _deltaTime)
{
	for (int i = 0; i < m_pASEParser->GetMeshNum(); i++)
	{
		mMeshList[i]->Update(pCamera, _deltaTime);
	}

	/// �ٲ� ������ �������� ���� ����
	for (int i = 0; i < m_pASEParser->GetMeshNum(); i++)
	{
		mMeshList[i]->UpdateAnimation(_deltaTime);
		mMeshList[i]->GetMesh()->m_WorldTM = WorldTM(mMeshList[i]);
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
	/// parser ���� ������ ���� ������
	//for (int i = 0; i < m_pASEParser->GetMeshNum(); i++)
	//{
	//	for (auto& v : mMeshList)
	//	{
	//		if (mMeshList[i]->GetMesh()->m_nodeparent == v->GetMesh()->m_nodename)
	//		{
	//			mMeshList[i]->GetMesh()->m_LocalTM = mMeshList[i]->GetMesh()->m_WorldTM * v->GetMesh()->m_WorldTM.Invert();
	//		}
	//	}
	//}

	// index�� ������ ���ϸ� ���� �ڽĵ鵵 ����tm �� ���ؾ��ϴϱ�
	// ��ü������ �ٽ� ���� tm �����ֱ�
	for (int i = 0; i < m_pASEParser->GetMeshNum(); i++)
	{
		mMeshList[i]->GetMesh()->m_WorldTM = WorldTM(mMeshList[i]);
		//mMeshList[i]->mWorld = mMeshList[i]->GetMesh()->m_WorldTM;
	}
}

/// <summary>
/// ������ ����� ��ȯ, ���带 ��ȯ
/// </summary>
/// <param name="mesh"></param>
/// <returns></returns>
Matrix Model::WorldTM(MeshObject* mesh)
{
	for (auto& v : mMeshList)
	{
		if (mesh->GetMesh()->m_nodeparent == v->GetMesh()->m_nodename)
			return mesh->GetMesh()->m_LocalTM * WorldTM(v);
	}

	return mesh->GetMesh()->m_WorldTM;
}

