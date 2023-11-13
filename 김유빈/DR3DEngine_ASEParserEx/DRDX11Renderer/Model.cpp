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
	m_pASEParser->Load((LPSTR)"../ASEFile/Cylinder.ASE");
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

	//for (int i = 0; i < m_pASEParser->GetMeshNum(); i++)
	//{
	//	MeshObject* mesh1 = mMeshList[i];

	//	for (int j = 0; j < m_pASEParser->GetMeshNum(); j++)
	//	{
	//		MeshObject* mesh2 = mMeshList[j];

	//		for (auto bone : mMeshList[j]->GetMesh()->m_vector_bone_list)
	//		{
	//			if (bone->m_bone_name == mesh1->GetMesh()->m_nodename)

	//		}
	//	}
	//}


	for (int i = 0; i < m_pASEParser->GetMeshNum(); i++)
	{
		// �޽� ���� ����, �ε��� ���� ����
		mMeshList[i]->LoadGeomerty();
	}

	SetChild();
}

void Model::Update(DRCamera* pCamera, float _deltaTime)
{
	for (int i = 0; i < m_pASEParser->GetMeshNum(); i++)
	{
		mMeshList[i]->Update(pCamera, _deltaTime);
	}

	/// node test
	//Matrix mRot, mLocal;
	//int index = 57;
	//for (int i = 0; i < m_pASEParser->GetMeshNum(); i++)
	//{
	//	if (mMeshList[i]->GetMesh()->m_nodename == "Bone01")
	//	{
	//		Mesh* m = mMeshList[i]->GetMesh();
	//		static float angle = 0;

	//		angle += 0.00001;
	//		m->m_LocalTM = Matrix::CreateRotationX(angle) * m->m_LocalTM;
	//	}
	//}

	for (int i = 0; i < m_pASEParser->GetMeshNum(); i++)
	{
		bool isNegative = mMeshList[i]->UpdateAnimation(_deltaTime);
	}

	SetHierarchy();
}

void Model::SetChild()
{
	// �ٷ� �� �ڽĸ�
	for (int i = 0; i < m_pASEParser->GetMeshNum(); i++)
	{
		for (auto child : mMeshList)
		{
			if (child->GetMesh()->m_nodeparent == mMeshList[i]->GetMesh()->m_nodename)
				mMeshList[i]->m_Children.push_back(child);
		}
	}

	// ���� �ڽ��� ���� �����ڵ� ���ϱ�
	for (int i = 0; i < m_pASEParser->GetMeshNum(); i++)
	{
		MeshObject* mesh = mMeshList[i];

		AddChildrenRecursive(mesh, mesh->m_Children);
	}
}

void Model::AddChildrenRecursive(MeshObject* mesh, std::vector<MeshObject*> children)
{
	for (MeshObject* grandchild : children)
	{
		mesh->m_Children.push_back(grandchild);
		AddChildrenRecursive(mesh, grandchild->m_Children);
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
	// index�� ������ ���ϸ� ���� �ڽĵ鵵 ����tm �� ���ؾ��ϴϱ�
	// ��ü������ �ٽ� ���� tm �����ֱ�
	for (int i = 0; i < m_pASEParser->GetMeshNum(); i++)
	{
		mMeshList[i]->GetMesh()->m_WorldTM = WorldTM(mMeshList[i]);
	}
}

/// <summary>
/// ������ ����� ��ȯ, ���带 ��ȯ
/// </summary>
/// <param name="mesh"></param>
/// <returns></returns>
Matrix Model::WorldTM(MeshObject* mesh)
{
	if (mesh->GetMesh()->m_nodeparent.empty())
	{
		mesh->GetMesh()->m_WorldTM = mesh->GetMesh()->m_LocalTM;
	}
	else
	{
		for (auto& v : mMeshList)
		{
			if (mesh->GetMesh()->m_nodeparent == v->GetMesh()->m_nodename)
				return mesh->GetMesh()->m_LocalTM * WorldTM(v);
		}
	}

	return mesh->GetMesh()->m_WorldTM;
}

