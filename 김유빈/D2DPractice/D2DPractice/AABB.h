#pragma once
#include <windows.h>
class MyRectangle;

class AABB
{
public:
	AABB();
	~AABB();

	bool CollisionCheck();
	void SetTarget(MyRectangle* _rect1, MyRectangle* _rect2);

	MyRectangle* rect1;
	MyRectangle* rect2;


};

