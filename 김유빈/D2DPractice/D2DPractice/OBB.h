#pragma once

class Vector2;
class BoundingBox;

class OBB
{
public:
	OBB();
	~OBB();

	bool CollisionCheck();
	void SetTarget(BoundingBox* _rect1, BoundingBox* _rect2);

	BoundingBox* rect1;
	BoundingBox* rect2;

	Vector2 center;		// 대충 두 도형의 중심을 잇는 벡터
	Vector2 normal;		// 분리축 후보군

private:

};

