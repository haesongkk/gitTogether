#include "Transform.h"
#include "GameObject.h"
#include <string>
#include <typeinfo>


Traditional::Transform::Transform()
	: LocalPosition(), LocalRotation(0), LocalScale(1.f, 1.f), m_pParent(nullptr)
{
	Identify();

	name = typeid(*this).name();
}

Traditional::Transform::~Transform()
{

}

void Traditional::Transform::Update()
{
	MakeTransformMatrix();
}

void Traditional::Transform::OnPreRender()
{
	SetTransform();
}

void Traditional::Transform::OnRender()
{

}

void Traditional::Transform::OnPostRender()
{

}

void Traditional::Transform::SetLocalPosition(CVECTOR2 position)
{
	LocalPosition = position;
	MakeTransformMatrix();
}

void Traditional::Transform::SetLocalRotation(float rotation)
{
	LocalRotation = rotation;
	MakeTransformMatrix();
}

void Traditional::Transform::SetLocalScale(CVECTOR2 scale)
{
	LocalScale = scale;
	MakeTransformMatrix();
}

CVECTOR2 Traditional::Transform::GetLocalPosition() const
{
	return LocalPosition;
}

float Traditional::Transform::GetLocalRotation() const
{
	return LocalRotation;
}

CVECTOR2 Traditional::Transform::GetLocalScale() const
{
	return LocalScale;
}

void Traditional::Transform::SetPosition(CVECTOR2 position)
{
	// 월드 좌표계의 포지션임
	LocalPosition = position * m_ParentsTM.Invert();
}

void Traditional::Transform::SetRotation(float rotation)
{

}

void Traditional::Transform::SetScale(CVECTOR2 scale)
{

}

CVECTOR2 Traditional::Transform::GetPosition()
{
	/*
	D2D1::Matrix3x2F _ParentTM = D2D1::Matrix3x2F::Identity();

	if (m_pParent != nullptr)
	{
		_ParentTM = m_pParent->GetTransformMatrix();
	}

	return LocalPosition * m_ParentsTM;
	*/
	MakeTransformMatrix();

	return GetPositionFromTM(m_TransformMatrix);
}

float Traditional::Transform::GetRotation()
{
	MakeTransformMatrix();

	return GetRotationFromTM(m_TransformMatrix);
}

CVECTOR2 Traditional::Transform::GetScale()
{
	MakeTransformMatrix();

	return GetScaleFromTM(m_TransformMatrix);
}

void Traditional::Transform::Translate(CVECTOR2 translation)
{
	LocalPosition += translation;
	MakeTransformMatrix();
}

void Traditional::Transform::Rotate(float angle)
{
	LocalRotation += angle;
	MakeTransformMatrix();
}

// 이동 변환 행렬을 만들어준다.
void Traditional::Transform::MakeTranslateMatrix(CVECTOR2 delta)
{
	m_TranslateMatrix = D2D1::Matrix3x2F::Translation(delta.x, delta.y);
}

// 회전 변환 행렬을 만들어준다.
void Traditional::Transform::MakeRotationMatrix(float angle, CVECTOR2 axis)
{
	m_RotationMatrix = D2D1::Matrix3x2F::Rotation(angle, D2D1::Point2F(axis.x, axis.y));
}

// 스케일 변환 행렬을 만들어준다.
void Traditional::Transform::MakeScaleMatrix(CVECTOR2 size, CVECTOR2 center)
{
	m_ScaleMatrix = D2D1::Matrix3x2F::Scale(size.x, size.y, D2D1::Point2F(center.x, center.y));
}

// 변환 행렬들을 초기화한다.
void Traditional::Transform::Identify()
{
	m_TranslateMatrix = D2D1::Matrix3x2F::Identity();
	m_RotationMatrix = D2D1::Matrix3x2F::Identity();
	m_ScaleMatrix = D2D1::Matrix3x2F::Identity();

	m_TransformMatrix = D2D1::Matrix3x2F::Identity();
}

// 최종 변환 행렬을 만들어준다.
void Traditional::Transform::MakeTransformMatrix()
{
	MakeTranslateMatrix(LocalPosition);
	MakeRotationMatrix(LocalRotation, CVECTOR2(0, 0));
	MakeScaleMatrix(LocalScale, CVECTOR2(0, 0));

	if (m_pParent != nullptr)
	{
		m_pParent->MakeTransformMatrix();
		m_ParentsTM = m_pParent->GetTransformMatrix();
	}
	else
	{
		m_ParentsTM = D2D1::Matrix3x2F::Identity();
	}

	// 로컬 먼저, 부모 TM을 나중에
	m_RotationMatrix.dx = 1;
	m_RotationMatrix.dy = 1;

	D2D1::Matrix3x2F _temp = m_ScaleMatrix * m_RotationMatrix;

	m_TransformMatrix = m_ScaleMatrix * m_RotationMatrix * m_TranslateMatrix * m_ParentsTM;

	// 이 경우는 회전을 먼저, 스케일을 나중에 하므로 맞지 않다.
	//m_TransformMatrix = m_RotationMatrix * m_ScaleMatrix * m_TranslateMatrix * m_ParentsTM;

	// 이렇게 하면 완전히 다른 의미가 된다.
	//m_TransformMatrix = m_ParentsTM * m_ScaleMatrix * m_RotationMatrix * m_TranslateMatrix;
}

void Traditional::Transform::SetTransform()
{
	DRD2DEngine::GetInstance()->SetTransform(m_TransformMatrix);
}

CVECTOR2 Traditional::Transform::GetPositionFromTM(D2D1::Matrix3x2F TM)
{
	return CVECTOR2(TM.dx, TM.dy);
}

float Traditional::Transform::GetRotationFromTM(D2D1::Matrix3x2F TM)
{
	/// 1. m11과 m12를 가지고 atan을 이용해서 각도 구함
	/// sx * cos(theta) = m11, sx * sin(theta) = m12
	// 시계방향 회전이므로 m12의 값이 -sin(theta)가 아니라 sin(theta)이다.
	float _angle1 = atanf(1.0f * (TM.m12 / TM.m11)) * (180.0f / 3.1415f);

	/// 2. 회전행렬의 각 행이 단위벡터임을 이용함
	/// 각 행을 벡터로 보고 길이를 구하면 scale이 나옴
	/// 구한 scale으로 변환행렬의 m11또는 m22등을 나눠주면 회전행렬이 되므로
	/// cos나 sin의 역함수를 이용해서 각을 구함.
	/// dx라서 Transpose된 것을 고려해야 함
	CVECTOR2 _scale = GetScaleFromTM(TM);
	float _angle2 = acosf(TM.m11 / _scale.x) * (180.0f / 3.1415f);	// 각 행이 같은 스케일을 갖는다. (아래 참조)
	float _angle3 = asinf(TM.m12 / _scale.x) * (180.0f / 3.1415f);
	float _angle4 = asinf(-TM.m21 / _scale.y) * (180.0f / 3.1415f);
	float _angle5 = acosf(TM.m22 / _scale.y) * (180.0f / 3.1415f);

	/// TRS관련
	/// 스케일을 나눠주는 방식은, 스케일과 회전 곱 순서와, 벡터가 앞에 오느냐 뒤에 오느냐에 따라 다르다.(변환행렬은 전치가 된다)
	/// 이 경우는 DX이므로 (v)SRT이며, 그래서 변환행렬의 행이 같은 스케일을 갖는다.
	/// (v)SRT인 경우는 변환행렬의 각 행이 같은 스케일을 갖는다.
	/// TRS(v)인 경우는 변환행렬의 각 열이 같은 스케일을 갖는다.
	/// (이동 성분은 어쨌든 맨 마지막에 적용되게 된다.)

	/// (dx는 전치, ((VS)R)T)
	/// dx가 아닌경우 행렬의 곱셈 순서는 SRT의 적용 순서와 반대가 되게 된다.
	/// (T(R(SV)))

	return _angle1;
}

CVECTOR2 Traditional::Transform::GetScaleFromTM(D2D1::Matrix3x2F TM)
{
	CVECTOR2 _scale(0, 0);
	_scale.x = sqrtf(powf(TM.m11, 2.0f) + powf(TM.m12, 2.0f));
	_scale.y = sqrtf(powf(TM.m21, 2.0f) + powf(TM.m22, 2.0f));

	return _scale;
}

void Traditional::Transform::SetParent(Transform* parent)
{
	m_pParent = parent;
}

std::string Traditional::Transform::GetParentName()
{
	if (m_pParent == nullptr)
	{
		return std::string("null");
	}

	return m_pParent->m_pGameObject->GetName();
}
