#include <d3d11.h>
#include <directxtk/Simplemath.h>

#pragma comment (lib, "d3d11.lib")

#include "framework.h"
#include "Renderer.h"

using namespace DirectX::SimpleMath;

void Renderer::Create(HINSTANCE _hInst, int _width, int _height)
{
	// 1. ������ ����
	// 2. device, dc, swapchain ����
	// 3. ���� Ÿ�� �� ����
	// 4. ���� ���ٽ� �� ����
}

void Renderer::Run()
{
	// 1. ���� ť�� �����Ѵ�

	// 2. ���� Ÿ�� ��, ���� ���ٽ� �� Ŭ����
	m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView, Color{ 0.05f, 0.05f, 0.1f, 1.0f });
	m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	// 3. �⺻ ���� ���� ����
	m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// 4. ť�� �ִ� �͵� �ϳ��� �׸��� (ť? ����?)
	for (auto renderComp : m_renderQ)
		if (auto sp = renderComp.lock())
			sp->Render();

	// 5. ��
	m_pSwapChain->Present(0, 0);
}

void Renderer::AddRenderComponent(shared_ptr<RenderComp> _spRenderComp)
{
	for (auto rComp : m_renderQ)
		if (_spRenderComp = rComp.lock())
			return;
	m_renderQ.push_back(_spRenderComp);
}

void Renderer::DeleteRenderComponent(shared_ptr<RenderComp> _spRenderComp)
{
	for (auto it = m_renderQ.begin(); it < m_renderQ.end(); it++)
		if (_spRenderComp == (*it).lock())
			m_renderQ.erase(it);
}