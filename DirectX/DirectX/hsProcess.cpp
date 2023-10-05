#include "framework.h"
#include "hsProcess.h"

#include "hsSystem.h"

#include "hsWindow.h"
#include "hsRender.h"
#include "hsImGui.h"

hsProcess::hsProcess(HINSTANCE _hInst, UINT _width, UINT _height)
{
	m_pSystems.push_back(new hsWindow(
		_hInst, 
		_width, 
		_height));

	m_pSystems.push_back(new hsRender(
		GetSystem<hsWindow>()->GetHwnd(),
		_width, 
		_height));

	m_pSystems.push_back(new hsImGui(
		GetSystem<hsWindow>()->GetHwnd(),
		GetSystem<hsRender>()->GetDevice(),
		GetSystem<hsRender>()->GetDC()
	));


}

void hsProcess::Init()
{
	for (auto pSys : m_pSystems)
		pSys->Init();
}

void hsProcess::Update()
{
	while (true)
	{
		for (auto pSys : m_pSystems)
			pSys->Update();
	}
}

void hsProcess::Finalize()
{
	for (auto pSys : m_pSystems)
		pSys->Finalize();
}