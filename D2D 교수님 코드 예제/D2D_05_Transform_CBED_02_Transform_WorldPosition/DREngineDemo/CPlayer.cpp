#include "CPlayer.h"
#include "Transform.h"
#include <sstream>


CPlayer::CPlayer()
	: m_Velocity(0, 0), m_Angle(0), m_Scale(1.0f),
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

	// 유니티와 비슷한 사용법이 가능해졌다.
	Transform* _transform = GetComponent2<Transform*>();
	if (_transform != nullptr)
	{
		_transform->Translate(m_Velocity);
		_transform->Rotate(m_Angle);

		///Transform* _transform = gameObject->GetComponent2<Transform>();

		if (Key_SetPosition == true)
		{
			// 월드좌표계 기준의 위치 지정
			_transform->SetPosition(CVECTOR2(10, 10));
		}
	}

}

void CPlayer::KeyInput()
{
	// 키 입력
	if (GetAsyncKeyState(VK_UP))		Key_Up = true;		else Key_Up = false;
	if (GetAsyncKeyState(VK_DOWN))		Key_Down = true;	else Key_Down = false;
	if (GetAsyncKeyState(VK_RIGHT))		Key_Right = true;	else Key_Right = false;
	if (GetAsyncKeyState(VK_LEFT))		Key_Left = true;	else Key_Left = false;

	if (GetAsyncKeyState(VK_SPACE))		Key_Space = true;	else Key_Space = false;
	if (GetAsyncKeyState(VK_RETURN))	Key_Return = true;	else Key_Return = false;

	if (GetAsyncKeyState(VK__P))		Key_SetPosition = true;	else Key_SetPosition = false;

}

std::string CPlayer::GetDebugData_Name()
{
	Transform* _transform = gameObject->GetComponent2<Transform*>();

	std::stringstream ss;
	ss << "Name : " << gameObject->GetName().c_str() 
		<< " ParentsName : " << _transform->GetParentName();

	return ss.str();
}

std::string CPlayer::GetDebugData_LocalPos()
{
	Transform* _transform = gameObject->GetComponent2<Transform*>();
	CVECTOR2 _localPos = _transform->GetLocalPosition();

	std::stringstream ss;
	ss << gameObject->GetName().c_str() << " LPos : " 
		<< _localPos.x << "/" << _localPos.y;

	return ss.str();
}

std::string CPlayer::GetDebugData_WorldPos()
{
	Transform* _transform = gameObject->GetComponent2<Transform*>();
	CVECTOR2 _worldPos = _transform->GetPosition();

	std::stringstream ss;
	ss << gameObject->GetName().c_str() << " WPos : " 
		<< _worldPos.x << "/" << _worldPos.y;

	return ss.str();
}

std::string CPlayer::GetDebugData_Rotation()
{
	Transform* _transform = gameObject->GetComponent2<Transform*>();

	std::stringstream ss;
	ss << gameObject->GetName().c_str() << " LRot : "
		<< _transform->GetLocalRotation() << "  WRot : "
		<< _transform->GetRotation();

	return ss.str();
}

std::string CPlayer::GetDebugData_Scale()
{
	Transform* _transform = gameObject->GetComponent2<Transform*>();
	CVECTOR2 _worldScl = _transform->GetScale();

	std::stringstream ss;
	ss << gameObject->GetName().c_str() << " WScl : "
		<< _worldScl.x << "/" << _worldScl.y;

	return ss.str();
}

