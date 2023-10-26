#include "framework.h"
#include "Animation.h"
#include "GameObject.h"
#include "Node.h"

int Animation::fps = 30;

void Animation::Update()
{
	auto preTime = m_curTime;
	m_curTime = GetTickCount64() / 1000.f;

	auto deltaTime = m_curTime - preTime;
	m_timer += deltaTime;

	if (m_timer > m_keys[m_curKey].time / fps)
	{
		++m_curKey %= m_keys.size();
		if (m_curKey == 0) m_timer = 0;
	}
	
	auto nextKey = (m_curKey + 1) % m_keys.size();

	auto interval = (m_keys[nextKey].time - m_keys[m_curKey].time) / fps;
	auto ratio = (m_keys[nextKey].time / fps - m_timer) / interval;

	// º¸°£
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
