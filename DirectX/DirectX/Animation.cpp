#include "framework.h"
#include "Animation.h"
#include "Model.h"
#include "Node.h"
#include "Timer.h"

int Animation::fps = 30;

void Animation::Update()
{
	m_timer += Timer::GetInst()->GetDeltaTime();
	
	auto nextKey = (m_curKey + 1) % m_keys.size();
		
	if (m_timer > m_keys[nextKey].time / fps)
	{
		++m_curKey %= m_keys.size();
		++nextKey %= m_keys.size();
		if (nextKey == 0)
			m_timer -= m_keys.back().time / fps;
	}

	// º¸°£
	auto interval = (m_keys[nextKey].time - m_keys[m_curKey].time) / fps;
	auto ratio = (m_timer - m_keys[m_curKey].time / fps) / interval;

	Vector3 scale, pos;
	Quaternion rot;

	scale = Vector3::Lerp(
		m_keys[m_curKey].scale,
		m_keys[nextKey].scale,
		ratio);
	pos = Vector3::Lerp(
		m_keys[m_curKey].position,
		m_keys[nextKey].position,
		ratio);
	rot = Quaternion::Slerp(
		m_keys[m_curKey].rotation,
		m_keys[nextKey].rotation,
		ratio);
		
	m_pConnectNode->m_relativeMatrix =
		Matrix::CreateScale(scale)
		* Matrix::CreateFromQuaternion(rot)
		* Matrix::CreateTranslation(pos);
}
