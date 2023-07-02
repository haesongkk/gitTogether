#pragma once
#include <cmath>

struct Vector2 {
	float x;
	float y;


	Vector2() :
		x(0.f),
		y(0.f)
	{

	}

	Vector2(float x, float y)
	{
		this->x = x;
		this->y = y;
	}
	// 단위벡터를 만들기 위한 노말라이즈
	Vector2 Normalize()
	{
		Vector2 ret;
		float norm = (float)sqrt(pow(x, 2) + pow(y, 2));
		ret.x = x / norm;
		ret.y = y / norm;
		return ret;
	}
	// 내적
	float DotProduct(Vector2 v2)
	{
		float result = (this->x) * v2.x + (this->y) * v2.y;
		return result;
	}

	Vector2 operator+(Vector2 v2)
	{
		return Vector2(this->x + v2.x, this->y + v2.y);
	}

	Vector2 operator-(Vector2 v2)
	{
		return Vector2(this->x - v2.x, this->y - v2.y);
	}

	// 	Vector2(int x, int y)
	// 	{
	// 		this->x = x;
	// 		this->y = y;
	// 	}
};