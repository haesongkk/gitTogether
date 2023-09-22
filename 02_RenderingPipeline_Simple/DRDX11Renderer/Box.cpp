#include "Box.h"

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
	/// ������/������ �Ҵ�/������ �ݴ� ������
	ReleaseCOM(mFX);
	ReleaseCOM(mInputLayout);

	ReleaseCOM(mIB);
	ReleaseCOM(mVB);
}

void Box::Initialize()
{
	BuildGeometryBuffers();
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
	// �Է� ��ġ ��ü ����
	md3dImmediateContext->IASetInputLayout(mInputLayout);
	md3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// ���� ������Ʈ
	md3dImmediateContext->RSSetState(m_pRenderstate);

	// ���ؽ����ۿ� �ε������� ����
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	md3dImmediateContext->IASetVertexBuffers(0, 1, &mVB, &stride, &offset);
	md3dImmediateContext->IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);

	/// WVP TM���� ����
	// Set constants
	XMMATRIX view = XMLoadFloat4x4(&mView);
	XMMATRIX proj = XMLoadFloat4x4(&mProj);
	XMMATRIX world = XMLoadFloat4x4(&mWorld);
	XMMATRIX worldViewProj = world * view * proj;

	// ��� ���� ������ ���ؼ� ������������� ����� �������ش�. (��ü�� ĳ�ø� �ٲٴ� ���̸� GPU�� ������۰� �ٷ� ���ŵǴ� ���� �ƴϴ�.)
	mfxWorldViewProj->SetMatrix(reinterpret_cast<float*>(&worldViewProj));

	// ��ũ����...
	D3DX11_TECHNIQUE_DESC techDesc;
	mTech->GetDesc(&techDesc);

	// �����н���...
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		mTech->GetPassByIndex(p)->Apply(0, md3dImmediateContext);
		md3dImmediateContext->DrawIndexed(36, 0, 0);
	}
}

void Box::LoadFile()
{
	FS::path folderPath(L"../objfiles");

	assert(FS::exists(folderPath));			// ������ ���� ���
	assert(FS::is_directory(folderPath));

	float x = 0, y = 0;
	for (const auto& entry : FS::directory_iterator(folderPath))
	{
		const FS::path& filePath = entry.path();

		// ���� ������ �о� Ȯ���ڰ� obj �� ���
		if (FS::is_regular_file(filePath) && filePath.extension() == ".obj")
		{
			std::ifstream file(filePath);

			assert(file.is_open());

			std::string line;
			while (std::getline(file, line))
			{
				std::string  delimiter = ",";

				int pos = 0;

				std::string  token;

				Vertex objInfo;

				while (true)
				{
					pos = (int)line.find(delimiter);
					token = line.substr(0, pos);
					line.erase(0, pos + delimiter.length());

					// ������ ���� �� �д´�. �޸�(,) �� �������� ����. ���� 0 ���� �о��
					switch (count)
					{
						case 0:
							objInfo.Pos.x = (float)std::stoi(token);
							break;
						case 1:
							objInfo.Pos.y = (float)std::stoi(token);
							break;
						case 2:
							objInfo.Pos.z = (UINT)std::stoi(token);
							break;
						default:
							break;
					}

					if (pos == std::string::npos) break;
				}

				x = objInfo.posX;
				y = objInfo.posY;
			}

			file.close();
		}
	}
}

void Box::BuildGeometryBuffers()
{
	/// ���ؽ� ���۸� �����Ѵ�.

	/// ������ü�� ��� ���� �Ƹ��������, ��������� ���ų� �� ���� ����� ���̵��� 
	/// ��ġ�� ��¦ ������ ��ٸ������·� �������.

	Vertex vertices[] =
	{
		{ XMFLOAT3(-1.0f, -1.0f, -0.0f), XMFLOAT4((const float*)&Colors::White)   },
		{ XMFLOAT3(-1.0f, +1.0f, 0.0f), XMFLOAT4((const float*)&Colors::Black)   },
		{ XMFLOAT3(+1.0f, +1.0f, 0.0f), XMFLOAT4((const float*)&Colors::Red)     },	// ��� ����
		{ XMFLOAT3(+1.0f, -1.0f, 0.0f), XMFLOAT4((const float*)&Colors::Green)   },
		{ XMFLOAT3(-1.0f, -1.0f, +1.0f), XMFLOAT4((const float*)&Colors::Blue)    },
		{ XMFLOAT3(-1.0f, +1.0f, +1.0f), XMFLOAT4((const float*)&Colors::Yellow)  },
		{ XMFLOAT3(+1.0f, +1.0f, +1.0f), XMFLOAT4((const float*)&Colors::Cyan)    },
		{ XMFLOAT3(+1.0f, -1.0f, +1.0f), XMFLOAT4((const float*)&Colors::Magenta) }
	};

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Vertex) * 8;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = vertices;
	HR(md3dDevice->CreateBuffer(&vbd, &vinitData, &mVB));


	/// �ε��� ���۸� �����Ѵ�.

	UINT indices[] =
	{
		// front face
		0, 1, 2,
		0, 2, 3,

		// back face
		4, 6, 5,
		4, 7, 6,

		// left face
		4, 5, 1,
		4, 1, 0,

		// right face
		3, 2, 6,
		3, 6, 7,

		// top face
		1, 5, 6,
		1, 6, 2,

		// bottom face
		4, 0, 3,
		4, 3, 7
	};

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * 36;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = indices;
	HR(md3dDevice->CreateBuffer(&ibd, &iinitData, &mIB));
}

/// �� å ���������� �������� �ؼ� ������, ���⿡���� �̹� �����ϵ� ������ �о �����Ѵ�.
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

/// �������� �ϴ� ������ ��������� �־���.
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
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	// Create the input layout
	D3DX11_PASS_DESC passDesc;
	mTech->GetPassByIndex(0)->GetDesc(&passDesc);
	HR(md3dDevice->CreateInputLayout(vertexDesc, 2, passDesc.pIAInputSignature,
		passDesc.IAInputSignatureSize, &mInputLayout));
}
