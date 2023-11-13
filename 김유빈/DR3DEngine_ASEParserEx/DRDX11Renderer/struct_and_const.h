#ifndef VERTEX_H
#define VERTEX_H

//***************************************************************************************
// Vertex.h by Frank Luna (C) 2011 All Rights Reserved.
//
// Defines vertex structures and input layouts.
//***************************************************************************************


#include "DX11Define.h"
#include "d3dx11Effect.h"	// effect, tech

struct Vertex_PosColor
{
	XMFLOAT3 Pos;
	XMFLOAT4 Color;
};

namespace VertexStruct
{
	struct PosNormal
	{
		XMFLOAT3 Pos;
		XMFLOAT3 Normal;
	};

	/// 텍스쳐 예제로 추가. 이름은 용책과의 연관성을 위해서 그냥 유지함
	struct Basic32
	{
		XMFLOAT3 Pos;
		XMFLOAT3 Normal;
		XMFLOAT2 Tex;

		XMFLOAT3 weight;
		UINT32 boneIndexNum[4];
	};
}

class InputLayoutDesc
{
public:
	// Init like const int A::a[4] = {0, 1, 2, 3}; in .cpp file.
	static const D3D11_INPUT_ELEMENT_DESC PosNormal[2];

	/// 텍스쳐 예제로 추가. 이름은 용책과의 연관성을 위해서 그냥 유지함
	static const D3D11_INPUT_ELEMENT_DESC Basic32[3];

};

class InputLayouts
{
public:
	static void InitAll(ID3D11Device* device);
	static void DestroyAll();

	static ID3D11InputLayout* PosNormal;
	static ID3D11InputLayout* Basic32;		///
};

#endif // VERTEX_H
