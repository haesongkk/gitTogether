#pragma once

#include "DX11Define.h"
#include "d3dx11Effect.h"	// effect, tech
#include "struct_and_const.h"
#include "Effects.h"

class DRCamera;

/// <summary>
/// 메쉬로 된 오브젝트
/// 쉐이더 정리가 되기 전까지는 계속 스펙(IA, Shader)이 업그레이드 될 것이다.
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


	// 뭐뭐 있어야 하니..
private:
	ID3D11Device* md3dDevice;						// D3D11 디바이스
	ID3D11DeviceContext* md3dImmediateContext;		// 디바이스 컨텍스트

	ID3D11Buffer* mVB;
	ID3D11Buffer* mIB;

	// Transform Matrix
	XMFLOAT4X4 mWorld;	// Define transformations from local spaces to world space.
	XMFLOAT4X4 mView;
	XMFLOAT4X4 mProj;
	//XMMATRIX mView;
	//XMMATRIX mProj;

	// 와이어로 그리기 위한 RenderState. 쉐이더에서 해도 된다.
	// 예제에서 On/Off로 바꾸는 등의 일을 하는 경우 이것을 바꿔줘도 될 것 같다.
	// 쉐이더에서 하는 경우도 스위칭 비용도 줄일 수 있는 방법은 많다.
	ID3D11RasterizerState* m_pRenderstate;

	UINT mSkullIndexCount;


	DirectionalLight mDirLights[3];
	XMFLOAT3 mEyePosW;
	UINT mLightCount;

	Material mSkullMat;
	XMFLOAT4X4 mSkullWorld;


};

