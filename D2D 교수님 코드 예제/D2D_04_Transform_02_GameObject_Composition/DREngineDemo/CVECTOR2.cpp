#include "CVECTOR2.h"


CVECTOR2 CVECTOR2::operator+(const CVECTOR2& v)
{
	CVECTOR2 r;
	r.x = this->x + v.x;
	r.y = this->y + v.y;
	return r;
}

CVECTOR2 CVECTOR2::operator-(const CVECTOR2& v)
{
	CVECTOR2 r;
	r.x = this->x - v.x;
	r.y = this->y - v.y;
	return r;
}

CVECTOR2 CVECTOR2::operator*(const float& f)
{
	CVECTOR2 r;
	r.x = this->x * f;
	r.y = this->y * f;
	return r;
}

CVECTOR2 CVECTOR2::operator/(const float& f)
{
	CVECTOR2 r;
	r.x = this->x / f;
	r.y = this->y / f;
	return r;
}

CVECTOR2 CVECTOR2::operator+=(const CVECTOR2& v)
{
	this->x = this->x + v.x;
	this->y = this->y + v.y;
	return *this;
}

float CVECTOR2::GetSqrMagnitude()
{
	return (float)(pow(x, 2) + pow(y, 2));
}

float CVECTOR2::GetMagnitude()
{
	return sqrtf(GetSqrMagnitude());
}