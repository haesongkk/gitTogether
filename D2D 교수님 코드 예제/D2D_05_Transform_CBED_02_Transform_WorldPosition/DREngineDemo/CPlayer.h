#pragma once
#include "Component.h"
#include "CVECTOR2.h"

using namespace CBEngine;

///
/// 플레이어 클래스
///
/// Component화 되었음
/// 엔진의 사용자는 이런 식으로 스크립트(컴포넌트)를 만들어서
/// 엔티티와 거기에 붙어있는 컴포넌트를 컨트롤함으로서
/// 게임 오브젝트를 구현한다.
///
/// [2020.4.11 LeHideHome]
///
class CPlayer : public Component
{
public:
	CPlayer();
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

