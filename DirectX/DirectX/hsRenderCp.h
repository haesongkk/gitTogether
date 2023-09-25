#pragma once
#include "hsComponent.h"
class hsRenderCp : public hsComponent
{
public:
	virtual void Init() override;
	virtual void Update() override;
	virtual void Finalize() override;

private:
	ID3D11DeviceContext* m_pDeviceContext = nullptr;
	ID3D11Buffer* m_pConstantBuffer = nullptr;

	ID3D11Buffer* m_pVertexBuffer = nullptr;
	UINT m_VertextBufferStride = 0;
	UINT m_VertextBufferOffset = 0;

	ID3D11Buffer* m_pIndexBuffer = nullptr;
	UINT m_indexCount = 0;

	ID3D11ShaderResourceView* m_pTextureRV = nullptr;

};

