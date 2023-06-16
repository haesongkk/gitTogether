#pragma once

#include "GameObject.h"
#include "CVECTOR2.h"

using namespace Traditional;

///
/// 플레이어 클래스
///
/// 기본적인 것들이 구현되어 있는 GameObject를 상속해서
/// 구체적인 게임 내의 오브젝트를 구현한다.
/// 
/// 속도, 회전값, 크기를 추가적으로 가지고 있다.
/// 키 입력도 가능하다.
///
/// [2020.4.11 LeHideHome]
class CPlayer : public GameObject
{
public:
	CPlayer(string name);
	virtual ~CPlayer();


	/// IComponentEventFunc Override
public:
	virtual void Update() override;

	// 필요한 것만 재정의해서 쓰면 된다.
	//virtual void OnPreRender() override;
	//virtual void OnRender() override;
	//virtual void OnPostRender() override;


private:
	// 매 프레임별 속도
	CVECTOR2 m_Velocity;

	// 회전 각도
	float m_Angle;

	// 크기
	float m_Scale;

private:
	// 키 입력
	void KeyInput();

private:
	bool Key_Up;
	bool Key_Down;
	bool Key_Left;
	bool Key_Right;

	bool Key_Space;
	bool Key_Return;

	bool Key_SetPosition;


	///--------------------------------------------------
	/// 디버그
public:
	string GetDebugData_Name();
	string GetDebugData_LocalPos();
	string GetDebugData_WorldPos();
	string GetDebugData_Rotation();
	string GetDebugData_Scale();


};

