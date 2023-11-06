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
		mMeshList.push_back(mesh);
	}

	m_Animations = m_pASEParser->GetAnimation();
	SetHierarchy();

	for (int i = 0; i < m_pASEParser->GetMeshNum(); i++)
	{
		// 메시 관련 정점, 인덱스 버퍼 설정
		mMeshList[i]->LoadGeomerty();
	}
}

void Model::Update(DRCamera* pCamera, float _deltaTime)
{
	/// animation
	m_AnimationTime[0] += _deltaTime;
	m_AnimationTime[1] += _deltaTime;
	m_AnimationTime[2] += _deltaTime;

	for (auto anim : m_Animations)
	{
		int nextKey[3] = { -1, -1, -1 };
		Matrix scale = Matrix::CreateScale(1.f,1.f,1.f);
		Matrix pos = Matrix::CreateTranslation(0.f,0.f,0.f);
		Matrix rot = Matrix::CreateRotationX(0.f) 
			* Matrix::CreateRotationY(0.f) 
			* Matrix::CreateRotationZ(0.f);
		const float fps = 1.f;

		//if (anim->m_position.size() != 0)
		//{
		//	nextKey[0] = (m_currentFrame[0] + 1) % anim->m_position.size();
		//	pos = Matrix::CreateTranslation(anim->m_position[m_currentFrame[0]]->m_pos);
		//	if (m_AnimationTime[0] > (anim->m_position[nextKey[0]])->m_time / fps)
		//	{
		//		++m_currentFrame[0] %= anim->m_position.size();
		//		++nextKey[0] %= anim->m_position.size();

		//		if (nextKey[0] == 0)
		//			m_AnimationTime[0] -= anim->m_position.back()->m_time / fps;
		//	}
		//}

		if (anim->m_rotation.size() != 0)
		{
			nextKey[1] = (m_currentFrame[1] + 1) % anim->m_rotation.size();
			rot = Matrix::CreateFromQuaternion(anim->m_rotation[m_currentFrame[1]]->m_rotQT_accumulation);

			if (m_AnimationTime[1] > (anim->m_rotation[nextKey[1]])->m_time / fps)
			{
				++m_currentFrame[1] %= anim->m_rotation.size();
				++nextKey[1] %= anim->m_rotation.size();

				if (nextKey[1] == 0)
					m_AnimationTime[1] -= anim->m_rotation.back()->m_time / fps;
			}
		}

		//if (anim->m_scale.size() != 0)
		//{
		//	nextKey[2] = (m_currentFrame[2] + 1) % anim->m_scale.size();
		//	scale = Matrix::CreateScale(anim->m_scale[m_currentFrame[2]]->m_scale);

		//	if (m_AnimationTime[2] > (anim->m_scale[nextKey[2]])->m_time / fps)
		//	{
		//		++m_currentFrame[2] %= anim->m_scale.size();
		//		++nextKey[2] %= anim->m_scale.size();

		//		if (nextKey[2] == 0)
		//			m_AnimationTime[2] -= anim->m_scale.back()->m_time / fps;
		//	}
		//}

		for (int i = 0; i < m_pASEParser->GetMeshNum(); i++)
		{
			//if (anim->m_position.size() == 0)
				pos = Matrix::CreateTranslation(mMeshList[i]->GetMesh()->m_LocalTM.Translation());

			if (anim->m_nodename == mMeshList[i]->GetMesh()->m_nodename)
			{
				Matrix convert = XMMatrixIdentity();


				//if (anim->m_scale.size() != 0)
				//if (anim->m_rotation.size() != 0)
				//if (anim->m_position.size() != 0)
					convert *= scale;
					convert *= rot;
					convert *= pos;

				mMeshList[i]->GetMesh()->m_LocalTM = convert;
			}
		}
	}

	// index의 로컬이 변하면 걔의 자식들도 월드tm 이 변해야하니까
	// 전체적으로 다시 월드 tm 구해주기
	for (int i = 0; i < m_pASEParser->GetMeshNum(); i++)
	{
		mMeshList[i]->GetMesh()->m_WorldTM = WorldTM(mMeshList[i]);
		mMeshList[i]->mWorld = mMeshList[i]->GetMesh()->m_WorldTM;
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

