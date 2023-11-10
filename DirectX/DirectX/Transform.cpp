#include "framework.h"
#include "Transform.h"

Transform::Transform(weak_ptr<GameObject> _wpOwnerObj)
	:Component(_wpOwnerObj)
	, m_worldTM{}
	, m_wpParentTransform{}
	, m_position{0.f,0.f,0.f}
	, m_scale{1.f,1.f,1.f}
	, m_rotate{0.f,0.f,0.f}
{
}

Transform::~Transform()
{
}

void Transform::Run()
{
	auto mScale = Matrix::CreateScale(m_scale);
	auto mRot = Matrix::CreateRotationX(m_rotate.x)
		* Matrix::CreateRotationY(m_rotate.y)
		* Matrix::CreateRotationZ(m_rotate.z);
	auto mTrans = Matrix::CreateTranslation(m_position);

	auto mBasis = XMMatrixIdentity();
	if (auto parent = m_wpParentTransform.lock()) mBasis = parent->GetWorldTM();

	m_worldTM = mScale * mRot * mTrans * mBasis;
}
