#pragma once


class Circle
{
public:
	// ���̴ϱ� ������ ��ǥ�� ���������� ��ƺ���
	int m_posx;
	int m_posy;
	int m_radius;

	void Update();

	Circle();
	~Circle();

	float m_moveSpeed;
	bool isClicked;

private:
};

