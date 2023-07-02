#pragma once


class Circle
{
public:
	// 원이니까 원점을 좌표의 기준점으로 잡아본다
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

