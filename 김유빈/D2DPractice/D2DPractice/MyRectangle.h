#pragma once

class MyRectangle
{
public:
	// ���̴ϱ� ������ ��ǥ�� ���������� ��ƺ���
	int m_Left;
	int m_Right;
	int m_Top;
	int m_Bottom;

	void Update();

	MyRectangle();
	~MyRectangle();

	float m_moveSpeed;

	void SetRectangle(int _Left, int _Right, int _Top, int _Bottom)
	{
		m_Left = _Left;
		m_Right = _Right;
		m_Top = _Top;
		m_Bottom = _Bottom;
	}

	bool isClicked;

private:
};

