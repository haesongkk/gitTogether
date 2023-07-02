#pragma once
#include <windows.h>
#include "Vector2.h"

//class CVECTOR2;

class BoundingBox
{
public:
	Vector2 m_leftTop;
	Vector2 m_rightTop;
	Vector2 m_leftBottom;
	Vector2 m_rightBottom;
	Vector2 centerPos;	// �߽��� �������� �̵���Ű�� ���� ����


	void Update();

	BoundingBox();
	~BoundingBox();

	float m_moveSpeed;

	void SetRectangle(Vector2 _leftTop, Vector2 _rightTop, Vector2 _leftBottom, Vector2 _rightBottom)
	{
		m_leftTop = _leftTop;
		m_rightTop = _rightTop;
		m_leftBottom = _leftBottom;
		m_rightBottom = _rightBottom;

		centerPos.x = (m_leftTop.x + m_rightBottom.x) / 2;
		centerPos.y = (m_leftTop.y + m_rightBottom.y) / 2;
	}

	bool isClicked;

private:
	Vector2 tempCenter;

	double temp;			// ���� �ӽ� �����ϱ� ���� ����
	double radian;
	double degree;		// cmath �� �ﰢ�Լ� ������ ������ ����.. ��ȯ �ʿ���
	//double cosValue;
	//double sinValue;

	//void ConvertToPoint();
	void Rotate(int dir);	// �ð� �ݽð� ȸ�� ������ �Է¹޴´�
};

