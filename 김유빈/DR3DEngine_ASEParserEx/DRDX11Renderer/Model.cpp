#include "Model.h"
#include "MeshObject.h"

#include "DRCamera.h"
#include "DDSTextureLoader.h"
#include "GeometryGenerator.h"
#include "../DR3DLib/CASEParser.h"
#include "../DR3DLib/Animation.h"


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
		m_NodeMesh.insert(pair<string, MeshObject*>(mesh->GetMesh()->m_nodename, mesh));
		mMeshList.push_back(mesh);
	}

	m_Animations = m_pASEParser->GetAnimation();
	SetHierarchy();

	for (int i = 0; i < m_pASEParser->GetMeshNum(); i++)
	{
		// 메시 관련 정점, 인덱스 버퍼 설정
		mMeshList[i]->LoadGeomerty();
		Vector3 pos, scale;
		Quaternion rot;

		mMeshList[i]->GetMesh()->m_LocalTM.Decompose(scale, rot, pos);
		mMeshList[i]->GetMesh()->m_RotationTM = Matrix::CreateFromQuaternion(rot);
		mMeshList[i]->GetMesh()->m_TranslateTM = Matrix::CreateTranslation(pos);
		mMeshList[i]->GetMesh()->m_ScaleTM = Matrix::CreateScale(scale);
	}
}

void Model::Update(DRCamera* pCamera, float _deltaTime)
{
	/// animation
	m_AnimationTime[0] += _deltaTime * 1000;
	m_AnimationTime[1] += _deltaTime * 1000;
	//m_AnimationTime[2] += _deltaTime;
	


	//static int frameCountPos = 0;
	//static int frameCountRot = 0;
	//static int frameCountScale = 0;

	for (auto anim : m_Animations)
	{
		if (!m_NodeMesh[anim->m_nodename]) continue;
		Mesh* mesh = m_NodeMesh[anim->m_nodename]->GetMesh();

		Matrix scale = XMMatrixIdentity();
		Matrix pos = XMMatrixIdentity();
		Matrix rot = XMMatrixIdentity();

		//if (anim->m_position.size() != 0)
		//{
		//	if (anim->m_position[frameCountPos]->m_time <= m_AnimationTime[0])
		//	{
		//		++frameCountPos %= anim->m_position.size();
		//	}

		//	pos = Matrix::CreateTranslation(anim->m_position[frameCountPos]->m_pos);
		//	mesh->m_WorldTM = scale * rot * pos;
		//}
		//if (anim->m_position.size() != 0)
			//pos = Matrix::CreateTranslation(anim->m_position[frameCountPos]->m_pos);

		if (anim->m_rotation.size() != 0)
		{
			if (anim->m_rotation[frameCountRot]->m_time <= m_AnimationTime[1])
			{
				++frameCountRot %= anim->m_rotation.size();
				//mesh->m_RotationTM = Matrix::CreateFromQuaternion(anim->m_rotation[frameCountRot]->m_rotQT_accumulation) * mesh->m_RotationTM;
			}


			rot = Matrix::CreateFromQuaternion(anim->m_rotation[frameCountRot]->m_rotQT_accumulation);

			Quaternion beforeQuaternion;

			if (frameCountRot == 0)
			{
				Vector3 pos, scale;
				Quaternion rot;

				mesh->m_LocalTM.Decompose(scale, rot, pos);
				mesh->m_RotationTM = Matrix::CreateFromQuaternion(rot);
				mesh->m_TranslateTM = Matrix::CreateTranslation(pos);
				mesh->m_ScaleTM = Matrix::CreateScale(scale);

				beforeQuaternion = rot;
			}
			else
			{
				beforeQuaternion = beforeQuaternion.CreateFromAxisAngle(anim->m_rotation[frameCountRot - 1]->m_rot, anim->m_rotation[frameCountRot - 1]->m_angle);
			}

			Quaternion currentQuaternion = currentQuaternion.CreateFromAxisAngle(anim->m_rotation[frameCountRot]->m_rot, anim->m_rotation[frameCountRot]->m_angle);

			// 값 누산해서 가지고 있기.
			anim->m_rotation[frameCountRot]->m_rotQT_accumulation = XMQuaternionMultiply(beforeQuaternion, currentQuaternion);

			mesh->m_RotationTM = Matrix::CreateFromQuaternion(anim->m_rotation[frameCountRot]->m_rotQT_accumulation);
		}

		mesh->m_LocalTM = mesh->m_ScaleTM * mesh->m_RotationTM * mesh->m_TranslateTM;
	}
	
	for (int i = 0; i < m_pASEParser->GetMeshNum(); i++)
	{
		mMeshList[i]->GetMesh()->m_WorldTM = WorldTM(mMeshList[i]);
		//mMeshList[i]->mWorld = mMeshList[i]->GetMesh()->m_WorldTM;
	}


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

	//int index = 57;
	//static float angle = 0;
	//
	//angle += 0.00001;
	//mLocal = XMMatrixRotationX(angle) * XMLoadFloat4x4(&(mMeshList[index]->GetMesh()->m_LocalTM));
	//XMStoreFloat4x4(&(mMeshList[index]->GetMesh()->m_LocalTM), mLocal);

	
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
	// index의 로컬이 변하면 걔의 자식들도 월드tm 이 변해야하니까
	// 전체적으로 다시 월드 tm 구해주기
	for (int i = 0; i < m_pASEParser->GetMeshNum(); i++)
	{
		mMeshList[i]->GetMesh()->m_WorldTM = WorldTM(mMeshList[i]);
		//mMeshList[i]->mWorld = mMeshList[i]->GetMesh()->m_WorldTM;
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

