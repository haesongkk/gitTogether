#include "MeshObject.h"

void Object::setVertex(vector<Vertex> _vert)
{
	D3D11_SAMPLER_DESC sampDesc = {};
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	m_pDevice->CreateSamplerState(&sampDesc, &m_pSamplerLinear);

	/// vertex bf
	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Vertex) * _vert.size();
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vinitData;
	Vertex* vertices = new Vertex[_vert.size()];
	for (int i = 0; i < _vert.size(); i++)
		vertices[i] = _vert[i];
	vinitData.pSysMem = vertices;
	HRESULT hr = (m_pDevice->CreateBuffer(&vbd, &vinitData, &m_VB));

	/// Index bf
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

void Object::render()
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	m_pDC->IASetVertexBuffers(0, 1, &m_VB, &stride, &offset);

	m_pDC->PSSetShaderResources(0, 1, &m_srv);
	m_pDC->PSSetSamplers(0, 1, &m_pSamplerLinear);
	// 
	// draw the vertex buffer to the back buffer
	m_pDC->Draw(m_size, 0);
}
