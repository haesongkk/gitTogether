#include "framework.h"
#include "Render.h"

void Render::Mesh::Update()
{
    dc->IASetVertexBuffers(0, 1, &pVertexBuffer, &vertextBufferStride, &vertextBufferOffset);
    dc->IASetIndexBuffer(pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);

    dc->DrawIndexed(indexCount, 0, 0);
}

void Render::Run()
{
    for (auto mesh : m_wpMeshes)
        mesh.lock()->Update();
}

