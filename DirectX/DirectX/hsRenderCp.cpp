#include "framework.h"
#include "hsRenderCp.h"

void hsRenderCp::Init()
{
}

void hsRenderCp::Update()
{
    //cb.mWorld = DirectX::XMMatrixTranspose(obj->GetMatrix());
    //m_pDeviceContext->UpdateSubresource(m_pConstantBuffer, 0, nullptr, &cb, 0, 0);

    m_pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &m_VertextBufferStride, &m_VertextBufferOffset);

    m_pDeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);

    m_pDeviceContext->PSSetShaderResources(0, 1, &m_pTextureRV);

    m_pDeviceContext->DrawIndexed(m_indexCount, 0, 0);
}

void hsRenderCp::Finalize()
{
}
