#pragma once
#include "Component.h"
#include "CVECTOR2.h"

using namespace CBEngine;

///
/// �÷��̾� Ŭ����
///
/// Componentȭ �Ǿ���
/// ������ ����ڴ� �̷� ������ ��ũ��Ʈ(������Ʈ)�� ����
/// ��ƼƼ�� �ű⿡ �پ��ִ� ������Ʈ�� ��Ʈ�������μ�
/// ���� ������Ʈ�� �����Ѵ�.
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

