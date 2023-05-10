#pragma once


#include <windows.h>
#include <iostream>

#include <vector>
using std::vector;


/// 
/// ������ �� �ҿ�Ǵ� �۾� �ð��� DT ��� �Ѵ�
///
/// [Ű �Ŵ����� �ʿ��� ����]
/// 
/// 1. �� Ű�� ���� ���� üũ�ϰ�, ������ Ű ���� ���� ������ ������ ���� ����
/// = ������ ����ȭ��� �Ѵ�
/// 
/// 2. Ű �Է� �̺�Ʈ�� �� ��ü������ �����ϱ� ����
/// = �� ���� �Է¸� ���� ������, ���� ���¸� ������ ������ ���
/// 


// Ű �Է� ���� ����
enum class KEY_STATE
{
	NONE,		// ������ ����	(UP -> UP)
	TAP,		// �� ���� ����	(UP -> DOWN)
	HOLD,		// ������ �ִ�	(DOWN -> DOWN)
	AWAY,		// �� �� ����	(DOWN -> UP)
};


/// <summary>
/// KEY �� ����.. 
/// </summary>
enum class KEY
{
	LEFT,
	RIGHT,
	UP,
	DOWN,
	Q,
	W,
	E,
	R,
	T,
	Y,
	U,
	I,
	O,
	P,
	A,
	S,
	D,
	F,
	G,
	H,
	J,
	K,
	L,
	Z,
	X,
	C,
	V,
	B,
	N,
	M,

	ALT,
	CTRL,
	LSHIFT,
	SPACE,
	ENTER,
	ESC,

	LAST,		// enum �� ��
};

struct tKeyInfo
{
	KEY_STATE	eState;		// Ű�� ���� �� (NONE, TAP, HOLD, AWAY)
	bool		bPrevPush;		// ���� �����ӿ����� Ű ���� (UP, DOWN)
};

class InputManager
{
public:
	InputManager();			// ������ -> new ���� �Ҹ�
	~InputManager();		// �Ҹ��� -> delete ���� �Ҹ�
	
	void InitInput();
	void UpdateInput();

	/// ���� ���� ��ȯ���� �Լ�
	// enum Ű ���� ���� ���°��� �����Ѵ�
	KEY_STATE GetKeyState(KEY _eKey)
	{
		return m_vecKey[(int)_eKey].eState;
	}



private:
	// vector �� �ε����� �� Ű ���� �ǹ��ϰ� �ȴ�
	vector<tKeyInfo> m_vecKey;

	// (�ӽ�) �������� ��Ŀ���� �˾Ƴ��� ���� �ڵ�
	HWND hMainWnd;
};

