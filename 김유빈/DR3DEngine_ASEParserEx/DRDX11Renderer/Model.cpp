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
		// �ļ� ��ȯ��
		m_pASEParser->ConvertAll(m_pASEParser->GetMesh(i));

		// �޽� ���� �ʱ�ȭ 
		MeshObject* mesh = new MeshObject(md3dDevice, md3dImmediateContext, m_pRenderstate);
		mesh->Initialize(m_pASEParser->GetMesh(i));
		mMeshList.push_back(mesh);
	}

	SetHierarchy();

	for (int i = 0; i < m_pASEParser->GetMeshNum(); i++)
	{
		// �޽� ���� ����, �ε��� ���� ����
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

	XMMATRIX mRot, mLocal;

	int index = 57;
	static float angle = 0;
	
	angle += 0.00001;
	mLocal = XMMatrixRotationX(angle) * XMLoadFloat4x4(&(mMeshList[index]->GetMesh()->m_LocalTM));
	XMStoreFloat4x4(&(mMeshList[index]->GetMesh()->m_LocalTM), mLocal);

	// index�� ������ ���ϸ� ���� �ڽĵ鵵 ����tm �� ���ؾ��ϴϱ�
	// ��ü������ �ٽ� ���� tm �����ֱ�
	for (int i = 0; i < m_pASEParser->GetMeshNum(); i++)
	{
		mMeshList[i]->GetMesh()->m_WorldTM = WorldTM(mMeshList[i]);
		mMeshList[i]->mWorld = mMeshList[i]->GetMesh()->m_WorldTM;
	}
}

void Model::SetHierarchy()
{
	for (int i = 0; i < m_pASEParser->GetMeshNum(); i++)
	{
		for (auto& v : mMeshList)
		{
			if (mMeshList[i]->GetMesh()->m_nodeparent == v->GetMesh()->m_nodename)
			{
				mMeshList[i]->GetMesh()->m_LocalTM = mMeshList[i]->GetMesh()->m_WorldTM * v->GetMesh()->m_WorldTM.Invert();
			}
		}
	}

	for (int i = 0; i < m_pASEParser->GetMeshNum(); i++)
	{
		mMeshList[i]->GetMesh()->m_WorldTM = WorldTM(mMeshList[i]);
		mMeshList[i]->mWorld = mMeshList[i]->GetMesh()->m_WorldTM;
	}
}

Matrix Model::WorldTM(MeshObject* mesh)
{
	for (auto& v : mMeshList)
	{
		if (mesh->GetMesh()->m_nodeparent == v->GetMesh()->m_nodename)
			return mesh->GetMesh()->m_LocalTM * WorldTM(v);
	}

	return mesh->GetMesh()->m_WorldTM;
}

