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

	Vector2 center;		// ���� �� ������ �߽��� �մ� ����
	Vector2 normal;		// �и��� �ĺ���

private:

};

