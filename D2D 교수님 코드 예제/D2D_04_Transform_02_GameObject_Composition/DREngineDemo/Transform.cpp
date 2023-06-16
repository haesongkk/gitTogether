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
	// ���� ��ǥ���� ��������
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

// �̵� ��ȯ ����� ������ش�.
void Traditional::Transform::MakeTranslateMatrix(CVECTOR2 delta)
{
	m_TranslateMatrix = D2D1::Matrix3x2F::Translation(delta.x, delta.y);
}

// ȸ�� ��ȯ ����� ������ش�.
void Traditional::Transform::MakeRotationMatrix(float angle, CVECTOR2 axis)
{
	m_RotationMatrix = D2D1::Matrix3x2F::Rotation(angle, D2D1::Point2F(axis.x, axis.y));
}

// ������ ��ȯ ����� ������ش�.
void Traditional::Transform::MakeScaleMatrix(CVECTOR2 size, CVECTOR2 center)
{
	m_ScaleMatrix = D2D1::Matrix3x2F::Scale(size.x, size.y, D2D1::Point2F(center.x, center.y));
}

// ��ȯ ��ĵ��� �ʱ�ȭ�Ѵ�.
void Traditional::Transform::Identify()
{
	m_TranslateMatrix = D2D1::Matrix3x2F::Identity();
	m_RotationMatrix = D2D1::Matrix3x2F::Identity();
	m_ScaleMatrix = D2D1::Matrix3x2F::Identity();

	m_TransformMatrix = D2D1::Matrix3x2F::Identity();
}

// ���� ��ȯ ����� ������ش�.
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

	// ���� ����, �θ� TM�� ���߿�
	m_RotationMatrix.dx = 1;
	m_RotationMatrix.dy = 1;

	D2D1::Matrix3x2F _temp = m_ScaleMatrix * m_RotationMatrix;

	m_TransformMatrix = m_ScaleMatrix * m_RotationMatrix * m_TranslateMatrix * m_ParentsTM;

	// �� ���� ȸ���� ����, �������� ���߿� �ϹǷ� ���� �ʴ�.
	//m_TransformMatrix = m_RotationMatrix * m_ScaleMatrix * m_TranslateMatrix * m_ParentsTM;

	// �̷��� �ϸ� ������ �ٸ� �ǹ̰� �ȴ�.
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
	/// 1. m11�� m12�� ������ atan�� �̿��ؼ� ���� ����
	/// sx * cos(theta) = m11, sx * sin(theta) = m12
	// �ð���� ȸ���̹Ƿ� m12�� ���� -sin(theta)�� �ƴ϶� sin(theta)�̴�.
	float _angle1 = atanf(1.0f * (TM.m12 / TM.m11)) * (180.0f / 3.1415f);

	/// 2. ȸ������� �� ���� ������������ �̿���
	/// �� ���� ���ͷ� ���� ���̸� ���ϸ� scale�� ����
	/// ���� scale���� ��ȯ����� m11�Ǵ� m22���� �����ָ� ȸ������� �ǹǷ�
	/// cos�� sin�� ���Լ��� �̿��ؼ� ���� ����.
	/// dx�� Transpose�� ���� ����ؾ� ��
	CVECTOR2 _scale = GetScaleFromTM(TM);
	float _angle2 = acosf(TM.m11 / _scale.x) * (180.0f / 3.1415f);	// �� ���� ���� �������� ���´�. (�Ʒ� ����)
	float _angle3 = asinf(TM.m12 / _scale.x) * (180.0f / 3.1415f);
	float _angle4 = asinf(-TM.m21 / _scale.y) * (180.0f / 3.1415f);
	float _angle5 = acosf(TM.m22 / _scale.y) * (180.0f / 3.1415f);

	/// TRS����
	/// �������� �����ִ� �����, �����ϰ� ȸ�� �� ������, ���Ͱ� �տ� ������ �ڿ� �����Ŀ� ���� �ٸ���.(��ȯ����� ��ġ�� �ȴ�)
	/// �� ���� DX�̹Ƿ� (v)SRT�̸�, �׷��� ��ȯ����� ���� ���� �������� ���´�.
	/// (v)SRT�� ���� ��ȯ����� �� ���� ���� �������� ���´�.
	/// TRS(v)�� ���� ��ȯ����� �� ���� ���� �������� ���´�.
	/// (�̵� ������ ��·�� �� �������� ����ǰ� �ȴ�.)

	/// (dx�� ��ġ, ((VS)R)T)
	/// dx�� �ƴѰ�� ����� ���� ������ SRT�� ���� ������ �ݴ밡 �ǰ� �ȴ�.
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
