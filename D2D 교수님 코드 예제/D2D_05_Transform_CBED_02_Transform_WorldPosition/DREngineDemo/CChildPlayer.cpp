#include "CChildPlayer.h"
#include "Transform.h"
#include <sstream>


CChildPlayer::CChildPlayer()
	: m_Velocity(0, 0), m_Angle(0), m_Scale(1.0f),
	Key_Up(false), Key_Down(false), Key_Left(false), Key_Right(false),
	Key_ButtonA(false), Key_ButtonB(false), Key_ButtonC(false)
{

}

CChildPlayer::~CChildPlayer()
{

}

void CChildPlayer::Update()
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

	if (Key_ButtonA == true)
	{
		m_Angle = 1.1f;
	}

	if (Key_ButtonB == true)
	{
		m_Angle = -1.1f;
	}

	if (Key_ButtonC == true)
	{
		m_Velocity = CVECTOR2(0, 0);
		m_Angle = 0;
	}

	// 유니티와 비슷한 사용법이 가능해졌다.
	Transform* _transform = GetComponent2<Transform*>();
	_transform->Translate(m_Velocity);
	_transform->Rotate(m_Angle);

	///Transform* _transform = gameObject->GetComponent2<Transform>();
}

void CChildPlayer::KeyInput()
{
	// 키 입력
	if (GetAsyncKeyState(VK__W))		Key_Up = true;		else Key_Up = false;
	if (GetAsyncKeyState(VK__S))		Key_Down = true;	else Key_Down = false;
	if (GetAsyncKeyState(VK__A))		Key_Left = true;	else Key_Left = false;
	if (GetAsyncKeyState(VK__D))		Key_Right = true;	else Key_Right = false;

	if (GetAsyncKeyState(VK__X))		Key_ButtonA = true;	else Key_ButtonA = false;
	if (GetAsyncKeyState(VK__C))		Key_ButtonB = true;	else Key_ButtonB = false;
	if (GetAsyncKeyState(VK__Z))		Key_ButtonC = true;	else Key_ButtonC = false;


}

std::string CChildPlayer::GetDebugData_Name()
{
	Transform* _transform = gameObject->GetComponent2<Transform*>();

	std::stringstream ss;
	ss << "Name : " << gameObject->GetName().c_str()
		<< " ParentsName : " << _transform->GetParentName();

	return ss.str();
}

std::string CChildPlayer::GetDebugData_LocalPos()
{
	Transform* _transform = gameObject->GetComponent2<Transform*>();
	CVECTOR2 _localPos = _transform->GetLocalPosition();

	std::stringstream ss;
	ss << gameObject->GetName().c_str() << " LPos : "
		<< _localPos.x << "/" << _localPos.y;

	return ss.str();
}

std::string CChildPlayer::GetDebugData_WorldPos()
{
	Transform* _transform = gameObject->GetComponent2<Transform*>();
	CVECTOR2 _worldPos = _transform->GetPosition();

	std::stringstream ss;
	ss << gameObject->GetName().c_str() << " WPos : "
		<< _worldPos.x << "/" << _worldPos.y;

	return ss.str();
}

std::string CChildPlayer::GetDebugData_Rotation()
{
	Transform* _transform = gameObject->GetComponent2<Transform*>();

	std::stringstream ss;
	ss << gameObject->GetName().c_str() << " LRot : "
		<< _transform->GetLocalRotation() << "  WRot : "
		<< _transform->GetRotation();

	return ss.str();
}

std::string CChildPlayer::GetDebugData_Scale()
{
	Transform* _transform = gameObject->GetComponent2<Transform*>();
	CVECTOR2 _worldScl = _transform->GetScale();

	std::stringstream ss;
	ss << gameObject->GetName().c_str() << " WScl : "
		<< _worldScl.x << "/" << _worldScl.y;

	return ss.str();
}

