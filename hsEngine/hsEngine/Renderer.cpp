#include <d3d11.h>
#include <directxtk/Simplemath.h>

#pragma comment (lib, "d3d11.lib")

#include "framework.h"
#include "Renderer.h"

using namespace DirectX::SimpleMath;

void Renderer::Create(HINSTANCE _hInst, int _width, int _height)
{
	// 1. À©µµ¿ì »ý¼º
	// 2. device, dc, swapchain »ý¼º
	// 3. ·»´õ Å¸°Ù ºä »ý¼º
	// 4. ±íÀÌ ½ºÅÙ½Ç ºä »ý¼º
}

void Renderer::Run()
{
	// 1. ·»´õ Å¥¸¦ Á¤·ÄÇÑ´Ù

	// 2. ·»´õ Å¸°Ù ºä, ±íÀÌ ½ºÅÙ½Ç ºä Å¬¸®¾î
	m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView, Color{ 0.05f, 0.05f, 0.1f, 1.0f });
	m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	// 3. ±âº» À§»ó ±¸Á¶ ¼³Á¤
	m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// 4. Å¥¿¡ ÀÖ´Â °Íµé ÇÏ³ª¾¿ ±×¸®±â (Å¥? º¤ÅÍ?)
	for (auto renderComp : m_renderQ)
		if (auto sp = renderComp.lock())
			sp->Render();

	// 5. ³¡
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