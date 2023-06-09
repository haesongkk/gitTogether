#pragma once

#include <math.h>


class CVECTOR2
{
public:
	CVECTOR2()
		: x(0), y(0)
	{
	}

	CVECTOR2(float x, float y)
		: x(x), y(y)
	{
	}

	float x;
	float y;

	CVECTOR2 operator+(const CVECTOR2& v);
	CVECTOR2 operator-(const CVECTOR2& v);

	CVECTOR2 operator*(const float& f);
	CVECTOR2 operator/(const float& f);

	CVECTOR2 operator+=(const CVECTOR2& v);


	float GetSqrMagnitude();
	float GetMagnitude();
};