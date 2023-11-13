#include "MeshObject.h"

#include "DRCamera.h"
#include "DDSTextureLoader.h"
#include "GeometryGenerator.h"
#include "..\DR3DLib\CASEParser.h"


MeshObject::MeshObject(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, ID3D11RasterizerState* pRS)
	: md3dDevice(pDevice), md3dImmediateContext(pDeviceContext), m_pRenderstate(pRS),
	mVB(nullptr), mIB(nullptr),
	mDiffuseMapSRV(0), mEyePosW(0.0f, 0.0f, 0.0f)
{
	XMStoreFloat4x4(&mTexTransform, XMMatrixIdentity());
}

MeshObject::~MeshObject()
{
	ReleaseCOM(mDiffuseMapSRV);

	ReleaseCOM(mIB);
	ReleaseCOM(mVB);
}

void MeshObject::Initialize(ASEParser::Mesh* meshData)
{
	mMeshData = meshData;
	mWorld = mMeshData->m_WorldTM;

	mDirLights[0].Ambient = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	mDirLights[0].Diffuse = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	mDirLights[0].Specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	mDirLights[0].Direction = XMFLOAT3(0.57735f, -0.57735f, 0.57735f);

	mDirLights[1].Ambient = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	mDirLights[1].Diffuse = XMFLOAT4(0.20f, 0.20f, 0.20f, 1.0f);
	mDirLights[1].Specular = XMFLOAT4(0.25f, 0.25f, 0.25f, 1.0f);
	mDirLights[1].Direction = XMFLOAT3(-0.57735f, -0.57735f, 0.57735f);

	mDirLights[2].Ambient = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	mDirLights[2].Diffuse = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	mDirLights[2].Specular = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	mDirLights[2].Direction = XMFLOAT3(0.0f, -0.707f, -0.707f);

	m_Material.Ambient = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	m_Material.Diffuse = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	m_Material.Specular = XMFLOAT4(0.8f, 0.8f, 0.8f, 16.0f);

	XMMATRIX skullScale = XMMatrixScaling(0.5f, 0.5f, 0.5f);
	XMMATRIX skullOffset = XMMatrixTranslation(0.0f, 1.0f, 0.0f);
	XMStoreFloat4x4(&mWorld, XMMatrixMultiply(skullScale, skullOffset));

	/// Texture
	ID3D11Resource* texResource = nullptr;
	HR(CreateDDSTextureFromFile(md3dDevice,
	//L"../ASEFile/000000002405_reverse.dds", &texResource, &mDiffuseMapSRV));
	L"../ASEFile/WoodCrate01.dds", & texResource, & mDiffuseMapSRV));

	//L"Textures/WoodCrate01.dds", & texResource, & mDiffuseMapSRV));
	assert(mDiffuseMapSRV);

	D3D11_SAMPLER_DESC sampDesc = {};
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	md3dDevice->CreateSamplerState(&sampDesc, &m_pSamplerLinear);

	ReleaseCOM(texResource)	// view saves reference
}

// ASE 파서에서 버텍스 정보, 인덱스 정보를 얻어온다.
void MeshObject::LoadGeomerty()
{
	ASEParser::Mesh* meshData = nullptr;
	meshData = mMeshData;

	Vector3 pos, scale;
	Quaternion rot;

	mMeshData->m_LocalTM.Decompose(scale, rot, pos);
	mMeshData->m_RotationTM = (rot);
	mMeshData->m_TranslateTM = (pos);
	mMeshData->m_ScaleTM = (scale);

	m_Animations = mMeshData->m_Animation;

	UINT tcount = 0;

	std::vector<VertexStruct::Basic32> vertices;
	std::vector<UINT> indices;

	if (meshData->m_IsHelper) 
		return;
	else if (meshData->m_type == eObjectType::eObjectType_Shape)
	{
		for (int i = 0; i < meshData->m_vector_shape_line.size(); i++)
		{
			for (auto ver : meshData->m_vector_shape_line[i]->m_shapevertex)
			{
				ASEParser::Vertex* v = new ASEParser::Vertex;
				v->m_pos = ver->m_pos;
				meshData->m_opt_vertex.push_back(v);
			}
		}

		VertexCount = meshData->m_opt_vertex.size();
		vertices.resize(VertexCount);

		for (int i = 0; i < VertexCount; i++)
		{
			vertices[i].Pos.x = meshData->m_opt_vertex[i]->m_pos.x;
			vertices[i].Pos.y = meshData->m_opt_vertex[i]->m_pos.y;
			vertices[i].Pos.z = meshData->m_opt_vertex[i]->m_pos.z;
		}

		D3D11_BUFFER_DESC vbd;
		vbd.Usage = D3D11_USAGE_IMMUTABLE;
		vbd.ByteWidth = sizeof(VertexStruct::Basic32) * VertexCount;
		vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vbd.CPUAccessFlags = 0;
		vbd.MiscFlags = 0;
		D3D11_SUBRESOURCE_DATA vinitData;
		vinitData.pSysMem = &vertices[0];
		HR(md3dDevice->CreateBuffer(&vbd, &vinitData, &mVB));
	}
	else
	{
		VertexCount = meshData->m_opt_vertex.size();
		vertices.resize(VertexCount);

		for (int i = 0; i < VertexCount; i++)
		{
			vertices[i].Pos.x = meshData->m_opt_vertex[i]->m_pos.x;
			vertices[i].Pos.y = meshData->m_opt_vertex[i]->m_pos.y;
			vertices[i].Pos.z = meshData->m_opt_vertex[i]->m_pos.z;

			vertices[i].Normal.x = meshData->m_opt_vertex[i]->m_normal.x;
			vertices[i].Normal.y = meshData->m_opt_vertex[i]->m_normal.y;
			vertices[i].Normal.z = meshData->m_opt_vertex[i]->m_normal.z;

			vertices[i].Tex.x = meshData->m_opt_vertex[i]->u;
			vertices[i].Tex.y = meshData->m_opt_vertex[i]->v;

			vertices[i].weight.x = meshData->m_opt_vertex[i]->m_bw[0];
			vertices[i].weight.y = meshData->m_opt_vertex[i]->m_bw[1];
			vertices[i].weight.z = meshData->m_opt_vertex[i]->m_bw[2];

			vertices[i].boneIndexNum[0] = meshData->m_opt_vertex[i]->m_boneIndexNum[0];
			vertices[i].boneIndexNum[1] = meshData->m_opt_vertex[i]->m_boneIndexNum[1];
			vertices[i].boneIndexNum[2] = meshData->m_opt_vertex[i]->m_boneIndexNum[2];
			vertices[i].boneIndexNum[3] = meshData->m_opt_vertex[i]->m_boneIndexNum[3];
		}

		tcount = meshData->m_mesh_numfaces + 1;

		IndexCount = 3 * tcount;
		std::vector<UINT> indices(IndexCount);
		for (UINT i = 0; i < tcount; ++i)
		{
			indices[i * 3 + 0] = meshData->m_opt_index[i].index[0];
			indices[i * 3 + 1] = meshData->m_opt_index[i].index[2];
			indices[i * 3 + 2] = meshData->m_opt_index[i].index[1];
		}

		D3D11_BUFFER_DESC vbd;
		vbd.Usage = D3D11_USAGE_IMMUTABLE;
		vbd.ByteWidth = sizeof(VertexStruct::Basic32) * VertexCount;
		vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vbd.CPUAccessFlags = 0;
		vbd.MiscFlags = 0;
		D3D11_SUBRESOURCE_DATA vinitData;
		vinitData.pSysMem = &vertices[0];
		HR(md3dDevice->CreateBuffer(&vbd, &vinitData, &mVB));

		//
		// Pack the indices of all the meshes into one index buffer.
		//

		D3D11_BUFFER_DESC ibd;
		ibd.Usage = D3D11_USAGE_IMMUTABLE;
		ibd.ByteWidth = sizeof(UINT) * IndexCount;
		ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		ibd.CPUAccessFlags = 0;
		ibd.MiscFlags = 0;
		D3D11_SUBRESOURCE_DATA iinitData;
		iinitData.pSysMem = &indices[0];
		HR(md3dDevice->CreateBuffer(&ibd, &iinitData, &mIB));
	}
}

void MeshObject::Update(DRCamera* pCamera, float _deltaTime)
{
	mWorld = mMeshData->m_WorldTM;
	//UpdateAnimation(_deltaTime);

	//XMStoreFloat4x4(&mWorld, XMMatrixIdentity());
	XMStoreFloat4x4(&mView, pCamera->View());
	XMStoreFloat4x4(&mProj, pCamera->Proj());
	//XMMATRIX world = XMLoadFloat4x4(&mWorld);
	//world = XMMatrixScaling(0.3f, 0.3f, 0.3f) * world;
	//XMStoreFloat4x4(&mWorld, world);

	//mWorld._11 = 10.0f;
	//mWorld._22 = 10.0f;
	//mWorld._33 = 10.0f;

	//mWorld._43 = 10.0f;

	//
	// Switch the number of lights based on key presses.
	//
	if (GetAsyncKeyState('0') & 0x8000)
		mLightCount = 0;

	if (GetAsyncKeyState('1') & 0x8000)
		mLightCount = 1;

	if (GetAsyncKeyState('2') & 0x8000)
		mLightCount = 2;

	if (GetAsyncKeyState('3') & 0x8000)
		mLightCount = 3;

	if (GetAsyncKeyState('4') & 0x8000)
		mLightCount = 4;

	if (GetAsyncKeyState('5') & 0x8000)
		mLightCount = 5;

	if (GetAsyncKeyState('6') & 0x8000)
		mLightCount = 6;

	if (GetAsyncKeyState('7') & 0x8000)
		mLightCount = 7;

	mEyePosW = XMFLOAT3(pCamera->GetPosition().x, pCamera->GetPosition().y, pCamera->GetPosition().z);
}

bool MeshObject::UpdateAnimation(float _deltaTime)
{
	/// animation
	// 총 누적 시간 계산
	m_AnimationTime[0] += _deltaTime * 1000;
	m_AnimationTime[1] += _deltaTime * 1000;
	m_AnimationTime[2] += _deltaTime * 1000;

	Quaternion rot;
	Vector3 pos;
	Vector3 scl;

	Quaternion preRotationQ;
	Quaternion curRotationQ;
	Quaternion nextRotationQ;

	/// Rotation
	if (m_Animations.m_rotation.size() != 0)
	{
		auto nextKey = (frameCountRot + 1) % m_Animations.m_rotation.size();

		// index 가 0 이면 이전값이 읍기 때문에 누적에 지금값을 걍 밖는다 + 다음 프레임을 위한 pre회전값에 저장
		if (frameCountRot == 0)
		{
			preRotationQ = preRotationQ.CreateFromAxisAngle(m_Animations.m_rotation[frameCountRot]->m_rot, m_Animations.m_rotation[frameCountRot]->m_angle);
			m_Animations.m_rotation[frameCountRot]->m_rotQT_accumulation = preRotationQ;

			// for slerp next data
			nextRotationQ = nextRotationQ.CreateFromAxisAngle(m_Animations.m_rotation[nextKey]->m_rot, m_Animations.m_rotation[nextKey]->m_angle);

			m_Animations.m_rotation[nextKey]->m_rotQT_accumulation = XMQuaternionMultiply(preRotationQ, nextRotationQ);
		}
		
		if (frameCountRot != 0)
		{
			// pre index rotation data
			preRotationQ = m_Animations.m_rotation[frameCountRot - 1]->m_rotQT_accumulation;
			// current index rotation data
			curRotationQ = curRotationQ.CreateFromAxisAngle(m_Animations.m_rotation[frameCountRot]->m_rot, m_Animations.m_rotation[frameCountRot]->m_angle);
			// calculate accumulation preQ * curQ
			m_Animations.m_rotation[frameCountRot]->m_rotQT_accumulation = XMQuaternionMultiply(preRotationQ, curRotationQ);

			/// (for slerp) calc next data
			nextRotationQ = nextRotationQ.CreateFromAxisAngle(m_Animations.m_rotation[nextKey]->m_rot, m_Animations.m_rotation[nextKey]->m_angle);

			m_Animations.m_rotation[nextKey]->m_rotQT_accumulation = XMQuaternionMultiply(m_Animations.m_rotation[frameCountRot]->m_rotQT_accumulation, nextRotationQ);
		}

		rot = (m_Animations.m_rotation[frameCountRot]->m_rotQT_accumulation);

		float interval;
		float ratio;

		//if (frameCountRot < anim.m_rotation.size() - 1)
		{
			interval = m_Animations.m_rotation[nextKey]->m_time - m_Animations.m_rotation[frameCountRot]->m_time;

			ratio = (m_AnimationTime[0] - m_Animations.m_rotation[frameCountRot]->m_time) / interval;

			rot = Quaternion::Slerp(m_Animations.m_rotation[frameCountRot]->m_rotQT_accumulation, m_Animations.m_rotation[nextKey]->m_rotQT_accumulation, ratio);
		}

		

		if (m_AnimationTime[0] > m_Animations.m_rotation[nextKey]->m_time)
		{
			++frameCountRot %= m_Animations.m_rotation.size();
			++nextKey %= m_Animations.m_rotation.size();

			if (nextKey == 0)
				m_AnimationTime[0] -= m_Animations.m_rotation.back()->m_time;
		}

		/// Negative Scale
		float determinant = DirectX::XMVectorGetX(DirectX::XMMatrixDeterminant(mMeshData->m_LocalTM));
		if (determinant < 0.f)
			mMeshData->m_ScaleTM = Vector3{ -1.f, -1.f, -1.f };
	}
	else
	{
		rot = mMeshData->m_RotationTM;
	}




	/// Translation
	if (m_Animations.m_position.size() != 0)
	{
		float interval;
		float ratio;
		auto nextKey = (frameCountPos + 1) % m_Animations.m_position.size();

		// 보간
		interval = m_Animations.m_position[nextKey]->m_time - m_Animations.m_position[frameCountPos]->m_time;
		ratio = (m_AnimationTime[1] - m_Animations.m_position[frameCountPos]->m_time) / interval;

		//pos = m_Animations.m_position[frameCountPos]->m_pos;
		pos = Vector3::Lerp(m_Animations.m_position[frameCountPos]->m_pos, m_Animations.m_position[nextKey]->m_pos, ratio);

		//if (mMeshData->m_nodename == "Bone01")
		//{
		//	static vector<Vector3> t;
		//	t.push_back(pos);
		//}

		if (m_AnimationTime[1] > m_Animations.m_position[nextKey]->m_time)
		{
			++frameCountPos %= m_Animations.m_position.size();
			++nextKey %= m_Animations.m_position.size();

			if (nextKey == 0)
				m_AnimationTime[1] -= m_Animations.m_position.back()->m_time;	// 근데 확인하니까 짜피 모든 애니메이션의 마지막 프레임은 57600 동일
		}
	}
	else
	{
		pos = mMeshData->m_TranslateTM;
	}

	/// Scale
	if (m_Animations.m_scale.size() != 0)
	{
		auto nextKey = (frameCountScale + 1) % m_Animations.m_scale.size();

		//if (frameCountScale != 0)
		{
			// 현재 애니메이션 데이터의 프레임 값 - 이전 애니 프레임 값
			float interval = m_Animations.m_scale[nextKey]->m_time - m_Animations.m_scale[frameCountScale]->m_time;
			float ratio = (m_AnimationTime[2] - m_Animations.m_scale[frameCountScale]->m_time) / interval;

			scl = Vector3::Lerp(m_Animations.m_scale[frameCountScale]->m_scale, m_Animations.m_scale[nextKey]->m_scale, ratio);
		}

		if (m_AnimationTime[2] > m_Animations.m_scale[nextKey]->m_time)
		{
			++frameCountScale %= m_Animations.m_scale.size();
			++nextKey %= m_Animations.m_scale.size();

			if (nextKey == 0)
				m_AnimationTime[2] -= m_Animations.m_scale.back()->m_time;
		}
	}
	else
	{
		scl = mMeshData->m_ScaleTM;
	}

	mMeshData->m_LocalTM = Matrix::CreateScale(scl)
		* Matrix::CreateFromQuaternion(rot)
		* Matrix::CreateTranslation(pos);

	Vector3 posLocal, scaleLocal;
	Quaternion rotLocal;

	mMeshData->m_LocalTM.Decompose(scaleLocal, rotLocal, posLocal);
	mMeshData->m_RotationTM = (rotLocal);
	mMeshData->m_TranslateTM = (posLocal);
	mMeshData->m_ScaleTM = (scaleLocal);

	return mMeshData->m_IsNegative;
}

void MeshObject::Render()
{
	if (mMeshData->m_is_skinningobject)
		return;

	mWorld = mMeshData->m_WorldTM;
	mInputLayout = InputLayouts::Basic32;

	// 입력 배치 객체 셋팅
	md3dImmediateContext->IASetInputLayout(InputLayouts::Basic32);
	md3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// 렌더 스테이트
	md3dImmediateContext->RSSetState(m_pRenderstate);

	// 버텍스버퍼와 인덱스버퍼 셋팅
	UINT stride = sizeof(VertexStruct::Basic32);
	UINT offset = 0;
	md3dImmediateContext->IASetVertexBuffers(0, 1, &mVB, &stride, &offset);
	md3dImmediateContext->IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);

	/// WVP TM등을 셋팅
	// Set constants
	XMMATRIX view = XMLoadFloat4x4(&mView);
	XMMATRIX proj = XMLoadFloat4x4(&mProj);
	XMMATRIX world = XMLoadFloat4x4(&mWorld);

	//world = XMMatrixScaling(0.3f, 0.3f, 0.3f) * world;

	XMMATRIX worldViewProj = world * view * proj;

	// 월드의 역행렬
	XMMATRIX worldInvTranspose = MathHelper::InverseTranspose(world);

	// Set per frame constants.
	Effects::BasicTexFX->SetDirLights(mDirLights);
	Effects::BasicTexFX->SetEyePosW(mEyePosW);

	// Figure out which technique to use.
	mTech = Effects::BasicTexFX->Light1Tech;
	switch (mLightCount)
	{
	case 1:
		mTech = Effects::BasicTexFX->Light1Tech;
		break;
	case 2:
		mTech = Effects::BasicTexFX->Light2Tech;
		break;
	case 3:
		mTech = Effects::BasicTexFX->Light3Tech;
		break;
	case 4:
		mTech = Effects::BasicTexFX->Light0TexTech;
		break;
	case 5:
		mTech = Effects::BasicTexFX->Light1TexTech;
		break;
	case 6:
		mTech = Effects::BasicTexFX->Light2TexTech;
		break;
	case 7:
		mTech = Effects::BasicTexFX->Light3TexTech;
		break;
	}

	// 테크닉은...
	D3DX11_TECHNIQUE_DESC techDesc;
	mTech->GetDesc(&techDesc);
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		md3dImmediateContext->IASetVertexBuffers(0, 1, &mVB, &stride, &offset);
		md3dImmediateContext->IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);

		// WVP는 위에서
		worldInvTranspose = MathHelper::InverseTranspose(world);

		Effects::BasicTexFX->SetWorld(world);
		Effects::BasicTexFX->SetWorldInvTranspose(worldInvTranspose);
		Effects::BasicTexFX->SetWorldViewProj(worldViewProj);
		Effects::BasicTexFX->SetTexTransform(XMLoadFloat4x4(&mTexTransform));
		Effects::BasicTexFX->SetMaterial(m_Material);
		Effects::BasicTexFX->SetDiffuseMap(mDiffuseMapSRV);

		mTech->GetPassByIndex(p)->Apply(0, md3dImmediateContext);

		// texture
		md3dImmediateContext->PSSetShaderResources(0, 1, &mDiffuseMapSRV);
		md3dImmediateContext->PSSetSamplers(0, 1, &m_pSamplerLinear);

		if (mMeshData->m_type == eObjectType::eObjectType_Shape)
		{
			md3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
			md3dImmediateContext->Draw(VertexCount, 0);
		}
		else
		{
			md3dImmediateContext->DrawIndexed(IndexCount, 0, 0);
		}
	}
}

void MeshObject::BuildVertexLayout()
{
	//// Create the vertex input layout.
	//D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	//{
	//	{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
	//	{"COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
	//};

	//// Create the input layout
	//D3DX11_PASS_DESC passDesc;
	//mTech->GetPassByIndex(0)->GetDesc(&passDesc);
	//HR(md3dDevice->CreateInputLayout(vertexDesc, 2, passDesc.pIAInputSignature,
	//	passDesc.IAInputSignatureSize, &mInputLayout));
}

void MeshObject::BuildGeometryBuffers2()
{
	GeometryGenerator::MeshData box;

	GeometryGenerator geoGen;
	geoGen.CreateBox(1.0f, 1.0f, 1.0f, box);

	// Cache the vertex offsets to each object in the concatenated vertex buffer.
	mBoxVertexOffset = 0;

	// Cache the index count of each object.
	mBoxIndexCount = box.Indices.size();

	// Cache the starting index for each object in the concatenated index buffer.
	mBoxIndexOffset = 0;

	UINT totalVertexCount = box.Vertices.size();

	IndexCount = mBoxIndexCount;

	//
	// Extract the vertex elements we are interested in and pack the
	// vertices of all the meshes into one vertex buffer.
	//

	std::vector<VertexStruct::Basic32> vertices(totalVertexCount);

	UINT k = 0;
	for (size_t i = 0; i < box.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos = box.Vertices[i].Position;
		vertices[k].Normal = box.Vertices[i].Normal;
		vertices[k].Tex = box.Vertices[i].TexC;
	}

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(VertexStruct::Basic32) * totalVertexCount;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &vertices[0];
	HR(md3dDevice->CreateBuffer(&vbd, &vinitData, &mVB));

	//
	// Pack the indices of all the meshes into one index buffer.
	//

	std::vector<UINT> indices;
	indices.insert(indices.end(), box.Indices.begin(), box.Indices.end());

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * IndexCount;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &indices[0];
	HR(md3dDevice->CreateBuffer(&ibd, &iinitData, &mIB));
}
