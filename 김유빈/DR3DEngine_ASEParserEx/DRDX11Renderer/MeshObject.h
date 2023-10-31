#pragma once

#include "DX11Define.h"
#include "d3dx11Effect.h"	// effect, tech
#include "struct_and_const.h"
#include "Effects.h"
#include "../DR3DLib/CParsingDataClass.h"


class DRCamera;
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
	void Initialize();
	void LoadGeomerty(ASEParser::Mesh* meshData);
	void Update(DRCamera* pCamera);
	void Render();


public:
	void BuildGeometryBuffers2();		// ���ϱ����κ��� ���ؽ�/�ε������۸� �����.


private:
	ID3D11Device* md3dDevice;						// D3D11 ����̽�
	ID3D11DeviceContext* md3dImmediateContext;		// ����̽� ���ؽ�Ʈ

	ID3D11Buffer* mVB;
	ID3D11Buffer* mIB;

	// Transform Matrix
	XMFLOAT4X4 mWorld;	// Define transformations from local spaces to world space.
	XMFLOAT4X4 mView;
	XMFLOAT4X4 mProj;

	// ����������Ʈ�� �ٱ����� �޾ƿ´�.
	ID3D11RasterizerState* m_pRenderstate;

	UINT IndexCount;
	Material m_Material;

	/// ���� �ܺο��� �޾ƿ����� ����
	DirectionalLight mDirLights[3];
	XMFLOAT3 mEyePosW;
	UINT mLightCount;


	///----------------------------------------------------------------------------------------------------
	/// �ؽ��Ŀ��� �߰��� �κ�
	ID3D11ShaderResourceView* mDiffuseMapSRV;
	ID3D11SamplerState* m_pSamplerLinear;

	XMFLOAT4X4 mTexTransform;
	XMFLOAT4X4 mBoxWorld;

	int mBoxVertexOffset;
	UINT mBoxIndexOffset;
	UINT mBoxIndexCount;
};
