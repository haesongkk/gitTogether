#pragma once
#include "hsComponent.h"
class hsTransformCp : public hsComponent
{
public:
	virtual void Init() override;
	virtual void Update() override;
	virtual void Finalize() override;

private:
	hsTransformCp* m_parentTransform = nullptr;
	Matrix m_matrix = {};

};

