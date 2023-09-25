#include "Box.h"
#include "ObjLoader.h"

#include <DirectXColors.h>

#include <d3dcompiler.h>	// fx compile
#include <istream>
#include <fstream>
#include <filesystem>
#include <string>
#include <cassert>

namespace FS = std::filesystem;

Box::Box(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, ID3D11RasterizerState* pRS)
	: md3dDevice(pDevice), md3dImmediateContext(pDeviceContext), m_pRenderstate(pRS),
	mVB(nullptr), mIB(nullptr), mFX(nullptr), mTech(nullptr), mfxWorldViewProj(nullptr), mInputLayout(nullptr),
	mWorld(), mView(), mProj()
{
}

Box::~Box()
{
	/// 릴리즈/삭제는 할당/생성의 반대 순서로
	ReleaseCOM(mFX);
	ReleaseCOM(mInputLayout);

	ReleaseCOM(mIB);
	ReleaseCOM(mVB);
}

void Box::Initialize()
{
	//LoadOBJ();
	BuildGeometryBuffers();
	//m_Loader->Load_MTL_File("../objfiles/untitled.mtl");

	BuildFX();
	BuildVertexLayout();
}

void Box::Update(const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& proj)
{
	XMStoreFloat4x4(&mWorld, world);
	XMStoreFloat4x4(&mView, view);
	XMStoreFloat4x4(&mProj, proj);
}

void Box::Render()
{
	// 입력 배치 객체 셋팅
	md3dImmediateContext->IASetInputLayout(mInputLayout);
	md3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// 렌더 스테이트
	md3dImmediateContext->RSSetState(m_pRenderstate);

	// 버텍스버퍼와 인덱스버퍼 셋팅
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	md3dImmediateContext->IASetVertexBuffers(0, 1, &mVB, &stride, &offset);
	//md3dImmediateContext->IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);

	/// WVP TM등을 셋팅
	// Set constants
	XMMATRIX view = XMLoadFloat4x4(&mView);
	XMMATRIX proj = XMLoadFloat4x4(&mProj);
	XMMATRIX world = XMLoadFloat4x4(&mWorld);
	XMMATRIX worldViewProj = world * view * proj;

	// 상수 버퍼 변수를 통해서 윌드뷰프로젝션 행렬을 셋팅해준다. (객체의 캐시를 바꾸는 것이며 GPU의 상수버퍼가 바로 갱신되는 것은 아니다.)
	mfxWorldViewProj->SetMatrix(reinterpret_cast<float*>(&worldViewProj));

	// 테크닉은...
	D3DX11_TECHNIQUE_DESC techDesc;
	mTech->GetDesc(&techDesc);

	// 렌더패스는...
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		mTech->GetPassByIndex(p)->Apply(0, md3dImmediateContext);
		md3dImmediateContext->Draw(3, 0);
	}
}

void Box::LoadOBJ()
{
	// 내부 파일을 읽어 확장자가 obj 일 경우
	std::ifstream file(L"../objfiles/untitled.obj");

	assert(file.is_open());

	std::string line;
	// 한줄씩 읽는다
	while (std::getline(file, line))
	{
		m_ReadPos = 0;

		std::string  delimiter = " ";

		std::string  token;

		while (true)
		{
			token = ReadToken(line, delimiter);	// 0부터 델리미터까지를 저장

			if (token == "o" || token == "#" || token == "mtllib" || token == "s" || token == "usemtl")
				break;

			if (token == "v")
			{
				XMFLOAT3 v;

				v.x = (float)std::stod(ReadToken(line, delimiter));
				v.y = (float)std::stod(ReadToken(line, delimiter));
				v.z = (float)std::stod(ReadToken(line, delimiter));

				m_VerticesPos.push_back(v);
			}

			if (token == "vt")
			{
				XMFLOAT2 vt;

				vt.x = (float)std::stod(ReadToken(line, delimiter));
				vt.y = (float)std::stod(ReadToken(line, delimiter));

				m_Textures.push_back(vt);
			}
			
			if (token == "vn")
			{
				XMFLOAT3 vn;

				vn.x = (float)std::stod(ReadToken(line, delimiter));
				vn.y = (float)std::stod(ReadToken(line, delimiter));
				vn.z = (float)std::stod(ReadToken(line, delimiter));

				m_Normals.push_back(vn);
			}

			if (token == "f")
			{
				std::string deli2 = "/";
				UINT vIndex[3], uvIndex[3], norIndex[3];
				vIndex[0] = std::stod(ReadToken(line, deli2));
				uvIndex[0] = std::stod(ReadToken(line, deli2));
				norIndex[0] = std::stod(ReadToken(line, delimiter));

				vIndex[1] = std::stod(ReadToken(line, deli2));
				uvIndex[1] = std::stod(ReadToken(line, deli2));
				norIndex[1] = std::stod(ReadToken(line, delimiter));

				vIndex[2] = std::stod(ReadToken(line, deli2));
				uvIndex[2] = std::stod(ReadToken(line, deli2));
				norIndex[2] = std::stod(ReadToken(line, delimiter));

				m_VertexIndex.push_back(vIndex[0]);
				m_VertexIndex.push_back(vIndex[1]);
				m_VertexIndex.push_back(vIndex[2]);

				m_TextureIndex.push_back(uvIndex[0]);
				m_TextureIndex.push_back(uvIndex[1]);
				m_TextureIndex.push_back(uvIndex[2]);

				m_NormalIndex.push_back(norIndex[0]);
				m_NormalIndex.push_back(norIndex[1]);
				m_NormalIndex.push_back(norIndex[2]);
			}

			if (m_ReadPos == std::string::npos)
				break;
		}
	}
	file.close();
}

std::string Box::ReadToken(std::string& _line, std::string& _deli)
{
	m_ReadPos = (int)_line.find(_deli);
	std::string token = _line.substr(0, m_ReadPos);	// 0부터 델리미터까지를 저장

	_line.erase(0, m_ReadPos + _deli.length());	// 토큰에 담긴 범위까지 지움?
	
	// TODO: 여기에 return 문을 삽입합니다.
	return token;
}

void Box::LoadMTL()
{
	//void c3dObjFile::ProcessMtl(char* szFileName)

	
		//char szBuf[1024];

		//FILE* fp;
		//fopen_s(&fp, "../objfiles/", "r");

		//char szMtlName[1024];

		////mtl 파일 분석해서 불러오는 부분
		//while (!feof(fp))
		//{
		//	fgets(szBuf, 1024, fp);
		//	if (szBuf[0] == '#') //주석 처리 부분은 패스하게끔.
		//	{
		//		continue;
		//	}
		//	else if (IsStartWith(szBuf, "newmtl")) //주석 처리 부분이 아니면 차근차근 읽어 나가서...
		//	{
		//		sscanf_s(szBuf, "%*s %s", szMtlName, 1024);
		//	}
		//	else if (IsStartWith(szBuf, "Ka"))
		//	{
		//		float fR, fG, fB;
		//		sscanf_s(szBuf, "%*s %f %f %f", &fR, &fG, &fB);
		//		m_MtlTexMap[szMtlName].Mtl.Ambient.r = fR;
		//		m_MtlTexMap[szMtlName].Mtl.Ambient.g = fG;
		//		m_MtlTexMap[szMtlName].Mtl.Ambient.b = fB;
		//		m_MtlTexMap[szMtlName].Mtl.Ambient.a = 1.0f;
		//	}
		//	else if (IsStartWith(szBuf, "Kd"))
		//	{
		//		float fR, fG, fB;
		//		sscanf_s(szBuf, "%*s %f %f %f", &fR, &fG, &fB);
		//		m_MtlTexMap[szMtlName].Mtl.Diffuse.r = fR;
		//		m_MtlTexMap[szMtlName].Mtl.Diffuse.g = fG;
		//		m_MtlTexMap[szMtlName].Mtl.Diffuse.b = fB;
		//		m_MtlTexMap[szMtlName].Mtl.Diffuse.a = 1.0f;
		//	}
		//	else if (IsStartWith(szBuf, "Ks"))
		//	{
		//		float fR, fG, fB;
		//		sscanf_s(szBuf, "%*s %f %f %f", &fR, &fG, &fB);
		//		m_MtlTexMap[szMtlName].Mtl.Specular.r = fR;
		//		m_MtlTexMap[szMtlName].Mtl.Specular.g = fG;
		//		m_MtlTexMap[szMtlName].Mtl.Specular.b = fB;
		//		m_MtlTexMap[szMtlName].Mtl.Specular.a = 1.0f;
		//	}
		//	else if (IsStartWith(szBuf, "Ke"))
		//	{
		//		float fR, fG, fB;
		//		sscanf_s(szBuf, "%*s %f %f %f", &fR, &fG, &fB);
		//		m_MtlTexMap[szMtlName].Mtl.Emissive.r = fR;
		//		m_MtlTexMap[szMtlName].Mtl.Emissive.g = fG;
		//		m_MtlTexMap[szMtlName].Mtl.Emissive.b = fB;
		//		m_MtlTexMap[szMtlName].Mtl.Emissive.a = 1.0f;
		//	}
		//	else if (IsStartWith(szBuf, "map_Kd"))
		//	{
		//		char szTextureFileName[1024];
		//		sscanf_s(szBuf, "%*s %s", szTextureFileName, 1024);

		//		//convert to unicode string
		//		wchar_t wszTextureFileName[256] = { 0, };
		//		::MultiByteToWideChar(CP_ACP, 0, szTextureFileName, -1, wszTextureFileName, strlen(szTextureFileName));

		//		D3DXCreateTextureFromFile(g_pD3DDevice, wszTextureFileName, &m_MtlTexMap[szMtlName].pTex);
		//	}
		//}
		//fclose(fp);
	
}

void Box::BuildGeometryBuffers()
{
	/// 버텍스 버퍼를 생성한다.

	/// 정육면체를 찍는 것이 아름답겠지만, 투영행렬이 없거나 할 때도 제대로 보이도록 
	/// 위치를 살짝 조정한 사다리꼴형태로 만들었다.
	
	//for (UINT i = 0; i < m_VertexIndex.size(); i++)
	//{
	//	UINT vindex = m_VertexIndex[i];
	//	UINT vtindex = m_TextureIndex[i];
	//	UINT vnindex = m_NormalIndex[i];

	//	Vertex vertex;
	//	vertex.Pos = m_VerticesPos[vindex - 1];
	//	vertex.Tex = m_Textures[vtindex - 1];
	//	vertex.Normal = m_Normals[vnindex - 1];

	//	m_Vertices.push_back(vertex);
	//}

	Vertex vertices[] =
	{
		{Vector3(-1.0f, -1.0f, -0.0f)  },
		{Vector3(-1.0f, +1.0f, 0.0f)},
		{ Vector3(+1.0f, -1.0f, +1.0f)}
	};

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Vertex) * 3;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = vertices;
	HR(md3dDevice->CreateBuffer(&vbd, &vinitData, &mVB));


	/// 인덱스 버퍼를 생성한다.

	//UINT indices[] =
	//{
	//	// front face
	//	0, 1, 2,
	//	0, 2, 3,

	//	// back face
	//	4, 6, 5,
	//	4, 7, 6,

	//	// left face
	//	4, 5, 1,
	//	4, 1, 0,

	//	// right face
	//	3, 2, 6,
	//	3, 6, 7,

	//	// top face
	//	1, 5, 6,
	//	1, 6, 2,

	//	// bottom face
	//	4, 0, 3,
	//	4, 3, 7
	//};

	//D3D11_BUFFER_DESC ibd;
	//ibd.Usage = D3D11_USAGE_IMMUTABLE;
	//ibd.ByteWidth = sizeof(UINT) * m_VertexIndex.size();
	//ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	//ibd.CPUAccessFlags = 0;
	//ibd.MiscFlags = 0;
	//ibd.StructureByteStride = 0;
	//D3D11_SUBRESOURCE_DATA iinitData;
	//UINT* indices = new UINT[m_VertexIndex.size()];
	//int tmp;
	//for (int i = 0; i < m_VertexIndex.size(); i++)
	//{
	//	if (i % 3 == 1) tmp = m_VertexIndex[i];
	//	if (i % 3 == 2)
	//	{
	//		m_VertexIndex[i - 1] = m_VertexIndex[i];
	//		m_VertexIndex[i] = tmp;
	//	}
	//}
	//for (int i = 0; i < m_VertexIndex.size(); i++)
	//	indices[i] = m_VertexIndex[i] - 1;
	//iinitData.pSysMem = indices;
	//HR(md3dDevice->CreateBuffer(&ibd, &iinitData, &mIB));
}

/// 용 책 예제에서는 컴파일을 해서 쓰지만, 여기에서는 이미 컴파일된 파일을 읽어서 생성한다.
void Box::BuildFX()
{
	std::ifstream fin("../debug/color.cso", std::ios::binary);

	fin.seekg(0, std::ios_base::end);
	int size = (int)fin.tellg();
	fin.seekg(0, std::ios_base::beg);
	std::vector<char> compiledShader(size);

	fin.read(&compiledShader[0], size);
	fin.close();

	HR(D3DX11CreateEffectFromMemory(&compiledShader[0], size,
		0, md3dDevice, &mFX));

	mTech = mFX->GetTechniqueByName("ColorTech");
	mfxWorldViewProj = mFX->GetVariableByName("gWorldViewProj")->AsMatrix();
}

/// 컴파일을 하는 버전도 참고용으로 넣었다.
void Box::BuildFX_Compile()
{
	DWORD shaderFlags = 0;
#if defined( DEBUG ) || defined( _DEBUG )
	shaderFlags |= D3D10_SHADER_DEBUG;
	shaderFlags |= D3D10_SHADER_SKIP_OPTIMIZATION;
#endif

	ID3D10Blob* compiledShader = 0;
	ID3D10Blob* compilationMsgs = 0;
	//HRESULT hr = D3DX11CompileFromFile(L"FX/color.fx", 0, 0, 0, "fx_5_0", shaderFlags, 0, 0, &compiledShader, &compilationMsgs, 0);
	HRESULT hr = D3DCompileFromFile(L"../FX/wire.fx", 0, 0, 0, "fx_5_0", shaderFlags, 0, &compiledShader, &compilationMsgs);

	// compilationMsgs can store errors or warnings.
	if (compilationMsgs != 0)
	{
		///MessageBoxA(0, (char*)compilationMsgs->GetBufferPointer(), 0, 0);
		ReleaseCOM(compilationMsgs);
	}

	// Even if there are no compilationMsgs, check to make sure there were no other errors.
	//if(FAILED(hr))
	//{
	//	DXTrace(__FILEW__, (DWORD)__LINE__, hr, L"D3DX11CompileFromFile", true);
	//}

	HR(D3DX11CreateEffectFromMemory(compiledShader->GetBufferPointer(), compiledShader->GetBufferSize(),
		0, md3dDevice, &mFX));

	// Done with compiled shader.
	ReleaseCOM(compiledShader);

	mTech = mFX->GetTechniqueByName("ColorTech");
	mfxWorldViewProj = mFX->GetVariableByName("gWorldViewProj")->AsMatrix();
}

void Box::BuildVertexLayout()
{
	// Create the vertex input layout.
	D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	// Create the input layout
	D3DX11_PASS_DESC passDesc;
	mTech->GetPassByIndex(0)->GetDesc(&passDesc);
	HR(md3dDevice->CreateInputLayout(vertexDesc, 3, passDesc.pIAInputSignature,
		passDesc.IAInputSignatureSize, &mInputLayout));
}
