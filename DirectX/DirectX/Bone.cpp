#include "framework.h"
#include "Bone.h"
#include "Node.h"
#include "Mesh.h"
#include "Model.h"
#include "Renderer.h"

void Bone::SetConnectNode(string name)
{
	for (auto node : m_pOwner->m_pOwner->m_pNodes)
		if (node->m_name == name)
			m_pConnectNode = node;
}

void Bone::Update()
{
	SetConnectNode(m_nodeName);
	assert(m_pConnectNode);
	Matrix matrix = m_offsetMatrix * m_pConnectNode->m_worldMatrix;
	m_pOwner->pRenderer->m_bones.bonePallete[m_index] = matrix.Transpose();
}
