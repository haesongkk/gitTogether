#include "framework.h"
#include "Animation.h"
#include "GameObject.h"
#include "Node.h"

void Animation::Update()
{
	if (false/*�ð� ������*/)
	{
		Matrix position, scale;
		Quaternion rotation;

		m_pConnectNode->m_matrix = Matrix::CreateScale(m_keys[0].scale)
			* Matrix::CreateFromQuaternion(m_keys[0].rotation)
			* Matrix::CreateTranslation(m_keys[0].scale);

	}

}
