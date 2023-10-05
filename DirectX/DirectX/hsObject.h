#pragma once
#include "hsBase.h"
class hsObject : public hsBase
{
public:
	virtual void Init() override;
	virtual void Update() override;
	virtual void Finalize() override;

public:
	struct Transform
	{
		Transform* parent = nullptr;
		Matrix matrix = {};

		Vector3 pos = {};
		Vector3 rotate = {};
		Vector3 scale = {};
	}*m_transform;

	struct Render
	{
		ID3D11Device* pDevice = nullptr;
		ID3D11DeviceContext* pDC = nullptr;

		ID3D11VertexShader* pVS = nullptr;
		ID3D11PixelShader* pPS = nullptr;
		ID3D11InputLayout* pIL = nullptr;

		ID3D11Buffer* pCB = nullptr;

		ID3D11Buffer* pVB = nullptr;
		UINT vertextBufferStride = 0;
		UINT vertextBufferOffset = 0;

		ID3D11Buffer* pIB = nullptr;
		UINT indexCount = 0;

		ID3D11SamplerState* pSL = nullptr;
		ID3D11ShaderResourceView* pTRV = nullptr;
	}*m_render;

private:

};

