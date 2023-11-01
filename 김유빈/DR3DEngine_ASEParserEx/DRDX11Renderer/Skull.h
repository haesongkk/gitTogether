#pragma once

#include "DX11Define.h"
#include "d3dx11Effect.h"	// effect, tech
#include "struct_and_const.h"
#include "Effects.h"

class DRCamera;

/// <summary>
/// �޽��� �� ������Ʈ
/// ���̴� ������ �Ǳ� �������� ��� ����(IA, Shader)�� ���׷��̵� �� ���̴�.
///
/// 2021.03.24 LeHideOffice
/// </summary>
class Skull
{
public:
	Skull(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, ID3D11RasterizerState* pRS);
	~Skull();

	struct Vertex
	{
		XMFLOAT3 Pos;
		XMFLOAT4 Color;
	};

public:
	void Initialize();
	void Update(DRCamera* pCamera);
	void Render();


private:
	void BuildGeometryBuffers();


	// ���� �־�� �ϴ�..
private:
	ID3D11Device* md3dDevice;						// D3D11 ����̽�
	ID3D11DeviceContext* md3dImmediateContext;		// ����̽� ���ؽ�Ʈ

	ID3D11Buffer* mVB;
	ID3D11Buffer* mIB;

	// Transform Matrix
	XMFLOAT4X4 mWorld;	// Define transformations from local spaces to world space.
	XMFLOAT4X4 mView;
	XMFLOAT4X4 mProj;
	//XMMATRIX mView;
	//XMMATRIX mProj;

	// ���̾�� �׸��� ���� RenderState. ���̴����� �ص� �ȴ�.
	// �������� On/Off�� �ٲٴ� ���� ���� �ϴ� ��� �̰��� �ٲ��൵ �� �� ����.
	// ���̴����� �ϴ� ��쵵 ����Ī ��뵵 ���� �� �ִ� ����� ����.
	ID3D11RasterizerState* m_pRenderstate;

	UINT mSkullIndexCount;


	DirectionalLight mDirLights[3];
	XMFLOAT3 mEyePosW;
	UINT mLightCount;

	Material mSkullMat;
	XMFLOAT4X4 mSkullWorld;


};

