#include "CPlayer.h"
#include "Transform.h"
#include <sstream>


CPlayer::CPlayer(string name)
	: GameObject(name),
	m_Velocity(0, 0), m_Angle(0), m_Scale(1.0f),
	Key_Up(false), Key_Down(false), Key_Left(false), Key_Right(false),
	Key_Space(false), Key_Return(false), Key_SetPosition(false)
{

}

CPlayer::~CPlayer()
{

}

void CPlayer::Update()
{
	// 키 입력. 실제 디바이스와 상태변수간의 연결
	KeyInput();

	// 오브젝트 이동
	if (Key_Up == true)
	{
		m_Velocity += CVECTOR2(0, -0.3f);
	}

	if (Key_Down == true)
	{
		m_Velocity += CVECTOR2(0, 0.3f);
	}

	if (Key_Right == true)
	{
		m_Velocity += CVECTOR2(0.3f, 0);
	}

	if (Key_Left == true)
	{
		m_Velocity += CVECTOR2(-0.3f, 0);
	}

	if (Key_Space == true)
	{
		m_Angle = 0.5f;
	}

	if (Key_Return == true)
	{
		m_Velocity = CVECTOR2(0, 0);
		m_Angle = 0;
	}

	// 정적으로 포함하고 있는 컴포넌트들
	if (m_pTransform != nullptr)
	{
		m_pTransform->Translate(m_Velocity);
		m_pTransform->Rotate(m_Angle);

		if (Key_SetPosition == true)
		{
			// 월드좌표계 기준의 위치 지정
			m_pTransform->SetPosition(CVECTOR2(10, 10));
		}
	}
}

void CPlayer::KeyInput()
{
	// 키 입력
	if (GetAsyncKeyState(VK_UP))	 Key_Up = true;		else Key_Up = false;
	if (GetAsyncKeyState(VK_DOWN))	 Key_Down = true;	else Key_Down = false;
	if (GetAsyncKeyState(VK_RIGHT))	 Key_Right = true;	else Key_Right = false;
	if (GetAsyncKeyState(VK_LEFT))	 Key_Left = true;	else Key_Left = false;

	if (GetAsyncKeyState(VK_SPACE))	 Key_Space = true;	else Key_Space = false;
	if (GetAsyncKeyState(VK_RETURN)) Key_Return = true;	else Key_Return = false;

	if (GetAsyncKeyState(VK__P))	 Key_SetPosition = true;	else Key_SetPosition = false;

}

std::string CPlayer::GetDebugData_Name()
{
	std::stringstream ss;
	ss << "Name : " << m_Name.c_str()
		<< " ParentsName : " << m_pTransform->GetParentName();

	return ss.str();
}

std::string CPlayer::GetDebugData_LocalPos()
{
	CVECTOR2 _localPos = m_pTransform->GetLocalPosition();

	std::stringstream ss;
	ss << m_Name.c_str() << " LPos : "
		<< _localPos.x << "/" << _localPos.y;

	return ss.str();
}

std::string CPlayer::GetDebugData_WorldPos()
{
	CVECTOR2 _worldPos = m_pTransform->GetPosition();

	std::stringstream ss;
	ss << m_Name.c_str() << " WPos : "
		<< _worldPos.x << "/" << _worldPos.y;

	return ss.str();
}

std::string CPlayer::GetDebugData_Rotation()
{
	std::stringstream ss;
	ss << m_Name.c_str() << " LRot : "
		<< m_pTransform->GetLocalRotation() << "  WRot : "
		<< m_pTransform->GetRotation();

	return ss.str();
}

std::string CPlayer::GetDebugData_Scale()
{
	CVECTOR2 _worldScl = m_pTransform->GetScale();

	std::stringstream ss;
	ss << m_Name.c_str() << " WScl : "
		<< _worldScl.x << "/" << _worldScl.y;

	return ss.str();
}

