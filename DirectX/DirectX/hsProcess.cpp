#include "framework.h"
#include "hsProcess.h"

#include "hsSystem.h"

#include "hsWndSys.h"
#include "hsRenderSys.h"
#include "hsUISys.h"

hsProcess::hsProcess(HINSTANCE _hInst, UINT _width, UINT _height)
{
	m_pSystems.push_back(new hsWndSys(
		_hInst, 
		_width, 
		_height));

	m_pSystems.push_back(new hsRenderSys(
		GetSystem<hsWndSys>()->GetHwnd(),
		_width, 
		_height));

	m_pSystems.push_back(new hsUISys(
		GetSystem<hsWndSys>()->GetHwnd(),
		GetSystem<hsRenderSys>()->GetDevice(),
		GetSystem<hsRenderSys>()->GetDC()
	));
}

void hsProcess::Init()
{
	for (auto pSys : m_pSystems)
		pSys->Init();
}

void hsProcess::Update()
{
	for (auto pSys : m_pSystems)
		pSys->Update();
}

void hsProcess::Finalize()
{
	for (auto pSys : m_pSystems)
		pSys->Finalize();
}