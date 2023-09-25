#pragma once

#include "DX11Define.h"
#include "./FL/d3dx11Effect.h"	// effect, tech

#include<map>
#include <vector>
#include <string>
#include <SimpleMath.h>
#include "LightHelper.h"
using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;



struct MetarialData
{
	Material metarial;
	ID3D11ShaderResourceView* srv;
};

class Object;
class ObjLoader
{
public:
	ObjLoader(ID3D11Device* _device, ID3D11DeviceContext* _dc);

	void Load_MTL_File(const char* path);
	void Load_OBJ_File(const char* path);

	void RenderParcingObj();
private:
	map<string, MetarialData*> mMetarial;
	ID3D11Device* g_pDevice;
	ID3D11DeviceContext* m_pDeviceContext;
	vector<Object*> vObject;
};

