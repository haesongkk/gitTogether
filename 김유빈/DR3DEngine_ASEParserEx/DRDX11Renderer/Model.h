#pragma once

#include "DX11Define.h"
#include "d3dx11Effect.h"	// effect, tech
#include "struct_and_const.h"
#include "Effects.h"
#include "../DR3DLib/CParsingDataClass.h"

#include <list>

class DRCamera;
class MeshObject;
class CASEParser;
class Animation;

class Model
{
public:
	Model(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, ID3D11RasterizerState* pRS);
	~Model();

public:
	void Initialize();
	void Update(DRCamera* pCamera, float _deltaTime);
	void Render();

private:
	void SetHierarchy();
	Matrix WorldTM(MeshObject* mesh);

	ID3D11Device* md3dDevice;						// D3D11 ����̽�
	ID3D11DeviceContext* md3dImmediateContext;		// ����̽� ���ؽ�Ʈ

	ID3D11Buffer* mVB;
	ID3D11Buffer* mIB;

	// Transform Matrix
	XMFLOAT4X4 mWorld;	// Define transformations from local spaces to world space.
	XMFLOAT4X4 mView;
	XMFLOAT4X4 mProj;
	XMFLOAT4X4 mLocalTM;	// NodeTM * inverse Parent's NodeTM 
	// ����������Ʈ�� �ٱ����� �޾ƿ´�.
	ID3D11RasterizerState* m_pRenderstate;

	UINT IndexCount;
	Material m_Material;

	/// ���� �ܺο��� �޾ƿ����� ����
	DirectionalLight mDirLights[3];
	XMFLOAT3 mEyePosW;
	UINT mLightCount;

	CASEParser* m_pASEParser;
	std::list<Animation*> m_Animations;
	std::vector<MeshObject*> mMeshList;

	float m_AnimationTime[3];
	int m_currentFrame[3];		// animkey
};

