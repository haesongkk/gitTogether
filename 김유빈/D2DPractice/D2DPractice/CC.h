#pragma once

// ������Ʈ�� ��ġ�� ���� ���� �ް� �浹 üũ
// �浹�̸�.. ������ ��ȯ�Ѵ�?

class Circle;

class CC
{
public:
	CC();
	~CC();

	//bool hasCollision;

	bool CollisionCheck();
	void SetTarget(Circle* c1, Circle* c2);

	Circle* c1;
	Circle* c2;

	int x_c1;
	int y_c1;
	int x_c2;
	int y_c2;

	int rad_c1;
	int rad_c2;
};

