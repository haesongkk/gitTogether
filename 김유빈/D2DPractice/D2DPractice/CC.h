#pragma once

// 오브젝트의 위치와 색상 값을 받고 충돌 체크
// 충돌이면.. 참값을 반환한다?

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

