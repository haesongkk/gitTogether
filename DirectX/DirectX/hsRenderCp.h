#pragma once
#include "hsComponent.h"
struct Vertex
{
	Vector3 position;
	Vector2 tex;
	Vector3 norm;

	Vertex() : position(Vector3()), norm(Vector3()), tex(Vector2()) { }

	Vertex(Vector3 position, Vector3 normal, Vector2 tex)
		: position(position), norm(normal), tex(tex) { }
};
struct ConstantBuffer
{
	Matrix mWorld;
	Matrix mView;
	Matrix mProjection;

	Vector4 lightDir;
	Vector4 lightColor;
};
class hsRenderCp : public hsComponent
{
public:
	virtual void Init() override;
	virtual void Update() override;
	virtual void Finalize() override;

	void CreateVertexShader(wstring& _filePath);
	void CreatePixelShader(wstring& _filePath);
		
	void CreateVertexBuffer(vector<Vertex>& _vertices, UINT _stride = 0);
	void CreateIndexBuffer(vector<UINT>& _indicies);
	
	void CreateTexture(wstring& _filePath);

	void CreateContantBuffer();

private:
	ID3D11Device* m_pDevice = nullptr;
	ID3D11DeviceContext* m_pDeviceContext = nullptr;

	ID3D11VertexShader* m_pVertexShader = nullptr;
	ID3D11PixelShader* m_pPixelShader = nullptr;
	ID3D11InputLayout* m_pInputLayout = nullptr;

	ID3D11Buffer* m_pConstantBuffer = nullptr;
	ConstantBuffer* m_pCB = nullptr;

	ID3D11Buffer* m_pVertexBuffer = nullptr;
	UINT m_VertextBufferStride = 0;
	UINT m_VertextBufferOffset = 0;

	ID3D11Buffer* m_pIndexBuffer = nullptr;
	UINT m_indexCount = 0;

	ID3D11SamplerState* m_pSamplerLinear = nullptr;
	ID3D11ShaderResourceView* m_pTextureRV = nullptr;

};

