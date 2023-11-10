#pragma once
#include "Component.h"
class Transform : public Component
{
	Matrix m_worldTM;
	weak_ptr<Transform> m_wpParentTransform;

	Vector3 m_position;
	Vector3 m_scale;
	Vector3 m_rotate;

public:
	Transform(weak_ptr<GameObject> _wpOwnerObj);

public:
	virtual ~Transform();

public:
	virtual void Run();

public:
	Matrix GetWorldTM() { return m_worldTM; }

};

