#pragma once


#include <windows.h>
#include <iostream>

//#include <vector>
//using std::vector;


/// 
/// 프레임 당 소요되는 작업 시간을 DT 라고 한다
///
/// [키 매니저가 필요한 이유]
/// 
/// 1. 각 키의 상태 값을 체크하고, 동일한 키 값에 대해 동일한 프레임 내에 적용
/// = 프레임 동기화라고 한다
/// 
/// 2. 키 입력 이벤트를 더 구체적으로 정의하기 위해
/// = 한 번의 입력만 받을 것인지, 누른 상태를 유지할 것인지 등등
/// 


// 키 입력 상태 정의
enum class KEY_STATE
{
	NONE,		// 눌리지 않은	(UP -> UP)
	TAP,		// 막 누른 시점	(UP -> DOWN)
	HOLD,		// 누르고 있는	(DOWN -> DOWN)
	AWAY,		// 막 뗀 시점	(DOWN -> UP)
};


/// <summary>
/// KEY 값 정의.. 
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

	LAST,		// enum 의 끝
};

struct KeyInfo
{
	KEY_STATE	keyState;		// 키의 상태 값 (NONE, TAP, HOLD, AWAY)
	bool		bPreDown;		// 이전 프레임에서의 키 상태 (UP, DOWN)
};

struct MOUSESTATE
{
	int x;
	int y;
	int wheel;
	bool left;
	bool right;
	bool middle;
};


class InputManager
{
public:
	void InitInput();
	void UpdateInput();

	/// 상태 값을 반환해줄 함수
	// enum 키 값에 따른 상태값을 리턴한다
	KEY_STATE GetKeyState(KEY _eKey)
	{
		return keyInfo[(int)_eKey].keyState;
	}

	static InputManager* GetInst();

private:
	// 생성자 소멸자를 감추고 인스턴스로 접근하게..
	InputManager();			// 생성자 -> new 에서 불림
	~InputManager();		// 소멸자 -> delete 에서 불림

	KeyInfo keyInfo[(int)KEY::LAST];

	MOUSESTATE curMouse;
	MOUSESTATE prevMouse;

	// (임시) 윈도우의 포커싱을 알아내기 위한 핸들
	//HWND hMainWnd;

	static InputManager* instance;
};

