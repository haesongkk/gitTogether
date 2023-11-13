#pragma once

#include "DX11Define.h"
#include "d3dx11Effect.h"	// effect, tech
#include "struct_and_const.h"
#include "Effects.h"
#include "../DR3DLib/CParsingDataClass.h"


class DRCamera;
class Animation;
//struct ASEParser::Mesh;

/// <summary>
/// �޽�
/// ���̴� ������ �Ǳ� �������� ��� ����(IA, Shader)�� ���׷��̵� �� ���̴�.
///
/// 2021.03.24 LeHideOffice
/// </summary>
class MeshObject
{
public:
	MeshObject(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, ID3D11RasterizerState* pRS);
	~MeshObject();

	struct Vertex
	{
		XMFLOAT3 Pos;
		XMFLOAT4 Color;
	};

public:
	void Initialize(ASEParser::Mesh* meshData);
	void LoadGeomerty();
	void Update(DRCamera* pCamera, float _deltaTime);
	bool UpdateAnimation(float _deltaTime);
	void Render();
	void BuildVertexLayout();

	ASEParser::Mesh* GetMesh() { return mMeshData; }

public:
	void BuildGeometryBuffers2();		// ���ϱ����κ��� ���ؽ�/�ε������۸� �����.
	XMFLOAT4X4 mWorld;	// Define transformations from local spaces to world space.
	vector<MeshObject*> m_Children;
	vector<MeshObject*> m_Bone;

private:
	/// Skinning Bone TM
	vector<XMFLOAT4X4> m_FinalBoneTM;

	float m_AnimationTime[3];
	int frameCountPos = 0;
	int frameCountRot = 0;
	int frameCountScale = 0;
	Animation m_Animations;

	ID3D11Device* md3dDevice;						// D3D11 ����̽�
	ID3D11DeviceContext* md3dImmediateContext;		// ����̽� ���ؽ�Ʈ

	ID3D11Buffer* mVB;
	ID3D11Buffer* mIB;

	// Transform Matrix
	XMFLOAT4X4 mView;
	XMFLOAT4X4 mProj;
	XMFLOAT4X4 mLocalTM;	// NodeTM * inverse Parent's NodeTM 
	// ����������Ʈ�� �ٱ����� �޾ƿ´�.
	ID3D11RasterizerState* m_pRenderstate;

	UINT IndexCount;
	UINT VertexCount;
	Material m_Material;

	/// ���� �ܺο��� �޾ƿ����� ����
	DirectionalLight mDirLights[3];
	XMFLOAT3 mEyePosW;
	UINT mLightCount;

	ASEParser::Mesh* mMeshData;

	///----------------------------------------------------------------------------------------------------
	/// �ؽ��Ŀ��� �߰��� �κ�
	ID3D11ShaderResourceView* mDiffuseMapSRV;
	ID3D11SamplerState* m_pSamplerLinear;
	ID3DX11EffectTechnique* mTech;
	ID3D11InputLayout* mInputLayout;

	XMFLOAT4X4 mTexTransform;
	XMFLOAT4X4 mBoxWorld;

	int mBoxVertexOffset;
	UINT mBoxIndexOffset;
	UINT mBoxIndexCount;
};
