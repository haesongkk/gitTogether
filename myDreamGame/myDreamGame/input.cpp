#include "input.h"

// Ű�� �ԷµǾ�����! �ԷµǾ��ٸ� true
bool keyTable[MAX_KEY];


/// ��, ��, ��, �� �� �ش� keyTable ���� �޾ƿ´�
bool GetKeyTable(const int keyIndex)
{
	return keyTable[keyIndex];
}

/// Ű ���� �� ������ �ٲ��ִ� �Լ�
void SetKeyTable(const int keyIndex, bool isOn)
{
	keyTable[keyIndex] = isOn;
}


/// GetAsyncKeyState ���
void UpdateInput()
{
	if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
	{
		SetKeyTable(ESCAPE, true);
	}

	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
	{
		SetKeyTable(SPACE, true);
	}

	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
	{
		SetKeyTable(LEFT, true);
	}

	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
	{
		SetKeyTable(RIGHT, true);
	}

	if (GetAsyncKeyState(VK_UP) & 0x8000)
	{
		SetKeyTable(UP, true);
	}

	if (GetAsyncKeyState(VK_DOWN) & 0x8000)
	{
		SetKeyTable(DOWN, true);
	}

}