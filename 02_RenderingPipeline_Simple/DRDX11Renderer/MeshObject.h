#pragma once
#include "DX11Define.h"
#include "./FL/d3dx11Effect.h"	// effect, tech

#include<map>
#include <string>
#include <vector>
#include <SimpleMath.h>
#include "LightHelper.h"
#include "Box.h"

using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;
class Object
{
public:
	Object(ID3D11Device* _dv, ID3D11DeviceContext* _dc) 
		: m_pDevice(_dv), m_pDC(_dc) { }
	void setVertex(vector<Vertex> _vert);
	void setVertexSize(int _size) { m_size = _size; }
	void setTexture(ID3D11ShaderResourceView* srv) { m_srv = srv; }
	void setMetarial(Material* _mat) { m_mat = _mat; }
	void setDirLight(DirectionalLight dir) { }

	void render();
private:
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDC;

	ID3D11Buffer* m_VB;	// 버텍스버퍼
	ID3D11Buffer* m_IB;	// 인덱스버퍼

	vector<Vertex> m_vert;
	int m_size;
	ID3D11ShaderResourceView* m_srv;
	Material* m_mat;

	ID3D11SamplerState* m_pSamplerLinear = nullptr;
	// 버텍스 인덱스 버퍼
	// 텍스처 버ㅓㅍ?
};

