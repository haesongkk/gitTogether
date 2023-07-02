#pragma once

struct Matrix3x3
{
	float _11, _12, _13;
	float _21, _22, _23;
	float _31, _32, _33;

	Matrix3x3():
		_11(0.f), _12(0.f), _13(0.f),
		_21(0.f), _22(0.f), _23(0.f),
		_31(0.f), _32(0.f), _33(0.f)
	{

	}

	Matrix3x3(
		float m11, float m12, float m13,
		float m21, float m22, float m23,
		float m31, float m32, float m33
	)
	{
		_11 = m11;
		_12 = m12;
		_13 = m13;
		_21 = m21;
		_22 = m22;
		_23 = m23;
		_31 = m31;
		_32 = m32;
		_33 = m33;
	}

	Matrix3x3 Identity()
	{
		Matrix3x3 identity;

		identity._11 = 1.f;
		identity._12 = 0.f;
		identity._13 = 0.f;
		identity._21 = 0.f;
		identity._22 = 1.f;
		identity._23 = 0.f;
		identity._31 = 0.f;
		identity._32 = 0.f;
		identity._33 = 1.f;

		return identity;
	}

};