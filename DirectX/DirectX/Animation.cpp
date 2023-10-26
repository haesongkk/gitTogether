#include "framework.h"
#include "Animation.h"
#include "GameObject.h"
#include "Node.h"

int Animation::fps = 24;

void Animation::Update()
{
	auto preTime = m_curTime;
	m_curTime = GetTickCount64() / 1000.f;
	auto deltaTime = m_curTime - preTime;
	m_timer += deltaTime;

	if (m_timer > m_keys[m_curKey].time / fps)
	{
		m_curKey++;
		m_curKey %= m_keys.size();
		if (m_curKey == 0) m_timer = 0;

		m_pConnectNode->m_relativeMatrix = Matrix::CreateScale(m_keys[m_curKey].scale)
			* Matrix::CreateFromQuaternion(m_keys[m_curKey].rotation)
			* Matrix::CreateTranslation(m_keys[m_curKey].position);
	}
}
