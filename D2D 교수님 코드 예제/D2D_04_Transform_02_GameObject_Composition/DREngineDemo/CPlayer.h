#pragma once

#include "GameObject.h"
#include "CVECTOR2.h"

using namespace Traditional;

///
/// �÷��̾� Ŭ����
///
/// �⺻���� �͵��� �����Ǿ� �ִ� GameObject�� ����ؼ�
/// ��ü���� ���� ���� ������Ʈ�� �����Ѵ�.
/// 
/// �ӵ�, ȸ����, ũ�⸦ �߰������� ������ �ִ�.
/// Ű �Էµ� �����ϴ�.
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

	// �ʿ��� �͸� �������ؼ� ���� �ȴ�.
	//virtual void OnPreRender() override;
	//virtual void OnRender() override;
	//virtual void OnPostRender() override;


private:
	// �� �����Ӻ� �ӵ�
	CVECTOR2 m_Velocity;

	// ȸ�� ����
	float m_Angle;

	// ũ��
	float m_Scale;

private:
	// Ű �Է�
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
	/// �����
public:
	string GetDebugData_Name();
	string GetDebugData_LocalPos();
	string GetDebugData_WorldPos();
	string GetDebugData_Rotation();
	string GetDebugData_Scale();


};

